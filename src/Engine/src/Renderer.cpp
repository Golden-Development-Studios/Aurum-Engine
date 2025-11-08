// --- Aurum Engine DirectX 12 Renderer Implementation ---
// Stage 3.3 – Command Queue & Swapchain Integration
// Adds RenderFrame() for a full per-frame clear + present cycle

#include <Engine/Renderer.hpp>
#include <directx/d3dx12.h>

using namespace Aurum;

Renderer::Renderer(HWND hwnd)
{
    Init(hwnd);
    Logger::Get().Log("DirectX 12 Renderer initialized.", LogLevel::Info);
}

Renderer::~Renderer()
{
    WaitForGPU();
    if (fenceEvent_)
        CloseHandle(fenceEvent_);

    Logger::Get().Log("Renderer shutdown complete.", LogLevel::Info);
}

void Renderer::Init(HWND hwnd)
{
    Logger::Get().Log("Initializing DirectX 12 context...", LogLevel::Info);

    Aurum::Render::DX12::ContextCreateDesc contextDesc{};
#if defined(_DEBUG)
    contextDesc.enableDebugLayer = true;
#else
    contextDesc.enableDebugLayer = false;
#endif

    // --- Initialize DX12Context (handles factory + adapter + device) ---
    if (!dx12Context_.Initialize(contextDesc))
    {
        Logger::Get().Log("Failed to initialize DX12 context.", LogLevel::Error);
        return;
    }

    device_ = dx12Context_.GetDevice();
    Logger::Get().Log("DX12 device acquired from context.", LogLevel::Info);

    // --- 2. Create command queue ---
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    HRESULT hr = device_->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue_));
    if (FAILED(hr))
    {
        Logger::Get().Log("Failed to create command queue.", LogLevel::Error);
        return;
    }

    // --- 3. Create swapchain ---
    ComPtr<IDXGIFactory6> factory = dx12Context_.GetFactory();
    DXGI_SWAP_CHAIN_DESC1 swapDesc = {};
    swapDesc.BufferCount = FrameCount;
    swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.Width = 1280;
    swapDesc.Height = 720;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> tempSwapChain;
    hr = factory->CreateSwapChainForHwnd(
        commandQueue_.Get(),
        hwnd,
        &swapDesc,
        nullptr,
        nullptr,
        &tempSwapChain
    );

    if (FAILED(hr))
    {
        Logger::Get().Log("Failed to create swapchain.", LogLevel::Error);
        return;
    }

    tempSwapChain.As(&swapChain_);
    frameIndex_ = swapChain_->GetCurrentBackBufferIndex();

    // --- 4. RTV heap ---
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    hr = device_->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap_));
    if (FAILED(hr))
    {
        Logger::Get().Log("Failed to create RTV heap.", LogLevel::Error);
        return;
    }

    rtvDescriptorSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // --- 5. Create RTVs for each backbuffer ---
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap_->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < FrameCount; i++)
    {
        swapChain_->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
        device_->CreateRenderTargetView(renderTargets_[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, rtvDescriptorSize_);
    }

    // --- 6. Command allocator & list ---
    device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
    device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
    commandList_->Close();

    // --- 7. Fence for GPU sync ---
    device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
    fenceValue_ = 1;
    fenceEvent_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    Logger::Get().Log("Renderer initialization completed successfully.", LogLevel::Info);
}

void Renderer::Clear(float r, float g, float b)
{
    commandAllocator_->Reset();
    commandList_->Reset(commandAllocator_.Get(), nullptr);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        rtvHeap_->GetCPUDescriptorHandleForHeapStart(),
        frameIndex_,
        rtvDescriptorSize_
    );

    // Transition to render target
    CD3DX12_RESOURCE_BARRIER toRT =
        CD3DX12_RESOURCE_BARRIER::Transition(
            renderTargets_[frameIndex_].Get(),
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET);

    commandList_->ResourceBarrier(1, &toRT);

    commandList_->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    const FLOAT clearColor[] = { r, g, b, 1.0f };
    commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // Transition back to present
    CD3DX12_RESOURCE_BARRIER toPresent =
        CD3DX12_RESOURCE_BARRIER::Transition(
            renderTargets_[frameIndex_].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT);

    commandList_->ResourceBarrier(1, &toPresent);

    commandList_->Close();
    ID3D12CommandList* lists[] = { commandList_.Get() };
    commandQueue_->ExecuteCommandLists(_countof(lists), lists);
}

// ================================================================
//  NEW SECTION — Stage 3.3 RenderFrame Integration
// ================================================================
void Renderer::RenderFrame()
{
    // 1. Reset command allocator & list
    commandAllocator_->Reset();
    commandList_->Reset(commandAllocator_.Get(), nullptr);

    // 2. Set current back buffer and RTV
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        rtvHeap_->GetCPUDescriptorHandleForHeapStart(),
        frameIndex_,
        rtvDescriptorSize_);

    auto* backBuffer = renderTargets_[frameIndex_].Get();

    // 3. Transition PRESENT → RENDER_TARGET
    CD3DX12_RESOURCE_BARRIER toRT =
        CD3DX12_RESOURCE_BARRIER::Transition(
            backBuffer,
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandList_->ResourceBarrier(1, &toRT);

    // 4. Clear color (demo blue)
    const FLOAT clearColor[4] = { 0.05f, 0.10f, 0.30f, 1.0f };
    commandList_->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // 5. Transition RENDER_TARGET → PRESENT
    CD3DX12_RESOURCE_BARRIER toPresent =
        CD3DX12_RESOURCE_BARRIER::Transition(
            backBuffer,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT);
    commandList_->ResourceBarrier(1, &toPresent);

    // 6. Execute and present
    commandList_->Close();
    ID3D12CommandList* lists[] = { commandList_.Get() };
    commandQueue_->ExecuteCommandLists(_countof(lists), lists);

    swapChain_->Present(1, 0);
    WaitForGPU();
    frameIndex_ = swapChain_->GetCurrentBackBufferIndex();
}
// ================================================================

void Renderer::Present()
{
    swapChain_->Present(1, 0);
    WaitForGPU();
    frameIndex_ = swapChain_->GetCurrentBackBufferIndex();
}

void Renderer::WaitForGPU()
{
    const UINT64 fenceToWaitFor = fenceValue_;
    commandQueue_->Signal(fence_.Get(), fenceToWaitFor);
    fenceValue_++;

    if (fence_->GetCompletedValue() < fenceToWaitFor)
    {
        fence_->SetEventOnCompletion(fenceToWaitFor, fenceEvent_);
        WaitForSingleObject(fenceEvent_, INFINITE);
    }
}
