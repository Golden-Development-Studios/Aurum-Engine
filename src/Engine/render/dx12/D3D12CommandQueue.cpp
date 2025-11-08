#include "D3D12CommandQueue.hpp"
#include <Framework/Logger.hpp>

using namespace Aurum::Render::DX12;

D3D12CommandQueue::~D3D12CommandQueue()
{
    Flush();
    if (m_fenceEvent)
    {
        CloseHandle(m_fenceEvent);
        m_fenceEvent = nullptr;
    }
}

bool D3D12CommandQueue::Initialize(ID3D12Device* device)
{
    if (!device) return false;

    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;

    if (FAILED(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_queue))))
    {
        Logger::Get().Log("DX12: Failed to create Command Queue.", LogLevel::Error);
        return false;
    }

    if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence))))
    {
        Logger::Get().Log("DX12: Failed to create Fence for Command Queue.", LogLevel::Error);
        return false;
    }

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    m_fenceValue = 1;

    Logger::Get().Log("DX12: Command Queue created.", LogLevel::Info);
    return true;
}

void D3D12CommandQueue::Execute(ID3D12CommandList* const* lists, UINT numLists)
{
    m_queue->ExecuteCommandLists(numLists, lists);
}

void D3D12CommandQueue::Signal()
{
    m_queue->Signal(m_fence.Get(), m_fenceValue++);
}

void D3D12CommandQueue::Flush()
{
    const UINT64 fenceToWaitFor = m_fenceValue;
    m_queue->Signal(m_fence.Get(), fenceToWaitFor);
    m_fenceValue++;

    if (m_fence->GetCompletedValue() < fenceToWaitFor)
    {
        m_fence->SetEventOnCompletion(fenceToWaitFor, m_fenceEvent);
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }
}
