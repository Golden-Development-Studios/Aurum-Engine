#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <Framework/Logger.hpp>

using Microsoft::WRL::ComPtr;

namespace Aurum
{
    class Renderer
    {
    public:
        Renderer(HWND hwnd);
        ~Renderer();

        void Clear(float r, float g, float b);
        void Present();

    private:
        void Init(HWND hwnd);
        void WaitForGPU();

        static const UINT FrameCount = 2;

        ComPtr<IDXGISwapChain3> swapChain_;
        ComPtr<ID3D12Device> device_;
        ComPtr<ID3D12CommandQueue> commandQueue_;
        ComPtr<ID3D12DescriptorHeap> rtvHeap_;
        ComPtr<ID3D12Resource> renderTargets_[FrameCount];
        ComPtr<ID3D12CommandAllocator> commandAllocator_;
        ComPtr<ID3D12GraphicsCommandList> commandList_;
        ComPtr<ID3D12Fence> fence_;
        HANDLE fenceEvent_ = nullptr;
        UINT rtvDescriptorSize_ = 0;
        UINT frameIndex_ = 0;
        UINT64 fenceValue_ = 0;
    };
}
