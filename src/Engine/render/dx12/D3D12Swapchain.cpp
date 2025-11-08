#include "D3D12Swapchain.hpp"
#include <Framework/Logger.hpp>
#include <directx/d3dx12.h>

using namespace Aurum::Render::DX12;

bool D3D12Swapchain::Initialize(
    IDXGIFactory4* factory,
    ID3D12Device* device,
    ID3D12CommandQueue* queue,
    HWND hwnd,
    UINT width,
    UINT height)
{
    if (!factory || !device || !queue || !hwnd) return false;

    m_device = device;

    DXGI_SWAP_CHAIN_DESC1 scDesc{};
    scDesc.Width       = width;
    scDesc.Height      = height;
    scDesc.Format      = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.BufferCount = kFrameCount;
    scDesc.SampleDesc  = { 1, 0 };
    scDesc.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scDesc.Scaling     = DXGI_SCALING_STRETCH;
    scDesc.AlphaMode   = DXGI_ALPHA_MODE_IGNORE;

    ComPtr<IDXGISwapChain1> temp;
    if (FAILED(factory->CreateSwapChainForHwnd(queue, hwnd, &scDesc, nullptr, nullptr, &temp)))
    {
        Logger::Get().Log("DX12: Failed to create Swapchain.", LogLevel::Error);
        return false;
    }

    temp.As(&m_swapchain);
    m_frameIndex = m_swapchain->GetCurrentBackBufferIndex();

    // Create RTV heap
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
    rtvHeapDesc.NumDescriptors = kFrameCount;
    rtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (FAILED(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap))))
    {
        Logger::Get().Log("DX12: Failed to create RTV descriptor heap.", LogLevel::Error);
        return false;
    }
    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // Create RTVs for back buffers
    CreateRTVs();

    Logger::Get().Log("DX12: Swapchain initialized (triple buffered).", LogLevel::Info);
    return true;
}

void D3D12Swapchain::CreateRTVs()
{
    m_backBuffers.resize(kFrameCount);
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvStart(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    auto handle = rtvStart;

    for (UINT i = 0; i < kFrameCount; ++i)
    {
        m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i]));
        m_device->CreateRenderTargetView(m_backBuffers[i].Get(), nullptr, handle);
        handle.Offset(1, m_rtvDescriptorSize);
    }
}

void D3D12Swapchain::Present(UINT syncInterval, UINT flags)
{
    m_swapchain->Present(syncInterval, flags);
    m_frameIndex = m_swapchain->GetCurrentBackBufferIndex();
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12Swapchain::GetCurrentRTV() const
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(
        m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
        static_cast<INT>(m_frameIndex),
        m_rtvDescriptorSize
    );
    return handle;
}

ID3D12Resource* D3D12Swapchain::GetCurrentRenderTarget() const
{
    return m_backBuffers[m_frameIndex].Get();
}
