#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>

using Microsoft::WRL::ComPtr;

namespace Aurum::Render::DX12
{
    class D3D12Swapchain
    {
    public:
        static constexpr UINT kFrameCount = 3; // Triple buffering

        D3D12Swapchain() = default;
        ~D3D12Swapchain() = default;

        bool Initialize(
            IDXGIFactory4* factory,
            ID3D12Device* device,
            ID3D12CommandQueue* queue,
            HWND hwnd,
            UINT width,
            UINT height);

        void Present(UINT syncInterval = 1, UINT flags = 0);

        // RTV access
        D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTV() const;
        ID3D12Resource*             GetCurrentRenderTarget() const;

        UINT                        GetCurrentBackBufferIndex() const { return m_frameIndex; }

        // (Future Stage 3.4) Resize support stub
        void Resize(UINT /*width*/, UINT /*height*/) {} // no-op for now

    private:
        void CreateRTVs();

        ComPtr<IDXGISwapChain3>           m_swapchain;
        ComPtr<ID3D12DescriptorHeap>      m_rtvHeap;
        std::vector<ComPtr<ID3D12Resource>> m_backBuffers;

        UINT m_frameIndex = 0;
        UINT m_rtvDescriptorSize = 0;

        ID3D12Device* m_device = nullptr;
    };
}
