// --- Aurum Engine DirectX 12 Renderer Implementation ---
// Includes DirectX helper structures and correct DX12 setup

#include <Engine/Renderer.hpp>
#include "directx/d3dx12.h"

namespace Aurum
{
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
        // 1. Create DXGI factory and device
        ComPtr<IDXGIFactory4> factory;
        CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));

        D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_));

        // 2. Create command queue
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;
        device_->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue_));

        // 3. Create swap chain
        DXGI_SWAP_CHAIN_DESC1 swapDesc = {};
        swapDesc.BufferCount = FrameCount;
        swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapDesc.Width = 1280;
        swapDesc.Height = 720;
        swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapDesc.SampleDesc.Count = 1;

        ComPtr<IDXGISwapChain1> tempSwapChain;
        factory->CreateSwapChainForHwnd(
            commandQueue_.Get(),
            hwnd,
            &swapDesc,
            nullptr,
            nullptr,
            &tempSwapChain
        );

        tempSwapChain.As(&swapChain_);
        frameIndex_ = swapChain_->GetCurrentBackBufferIndex();

        // 4. Create RTV heap
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = FrameCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        device_->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap_));
        rtvDescriptorSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        // 5. Create render target views
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap_->GetCPUDescriptorHandleForHeapStart());
        for (UINT i = 0; i < FrameCount; i++)
        {
            swapChain_->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
            device_->CreateRenderTargetView(renderTargets_[i].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, rtvDescriptorSize_);
        }

        // 6. Command allocator & list
        device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
        device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
        commandList_->Close();

        // 7. Fence for GPU sync
        device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
        fenceValue_ = 1;
        fenceEvent_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    }

    void Renderer::Clear(float r, float g, float b)
    {
        commandAllocator_->Reset();
        commandList_->Reset(commandAllocator_.Get(), nullptr);

        FLOAT clearColor[] = { r, g, b, 1.0f };
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
            rtvHeap_->GetCPUDescriptorHandleForHeapStart(),
            frameIndex_,
            rtvDescriptorSize_
        );

// --- Transition to render target ---
{
    CD3DX12_RESOURCE_BARRIER barrier =
        CD3DX12_RESOURCE_BARRIER::Transition(
            renderTargets_[frameIndex_].Get(),
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET);

    commandList_->ResourceBarrier(1, &barrier);
}

// --- Transition back to present ---
{
    CD3DX12_RESOURCE_BARRIER barrier =
        CD3DX12_RESOURCE_BARRIER::Transition(
            renderTargets_[frameIndex_].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT);

    commandList_->ResourceBarrier(1, &barrier);
}


        commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

// --- Transition to render target ---
{
    CD3DX12_RESOURCE_BARRIER barrier =
        CD3DX12_RESOURCE_BARRIER::Transition(
            renderTargets_[frameIndex_].Get(),
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET);

    commandList_->ResourceBarrier(1, &barrier);
}

// --- Transition back to present ---
{
    CD3DX12_RESOURCE_BARRIER barrier =
        CD3DX12_RESOURCE_BARRIER::Transition(
            renderTargets_[frameIndex_].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT);

    commandList_->ResourceBarrier(1, &barrier);
}


        commandList_->Close();

        ID3D12CommandList* lists[] = { commandList_.Get() };
        commandQueue_->ExecuteCommandLists(_countof(lists), lists);
    }

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
}
