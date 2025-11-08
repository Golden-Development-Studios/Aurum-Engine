#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

using Microsoft::WRL::ComPtr;

namespace Aurum::Render::DX12
{
    class D3D12CommandQueue
    {
    public:
        D3D12CommandQueue() = default;
        ~D3D12CommandQueue();

        bool Initialize(ID3D12Device* device);
        void Execute(ID3D12CommandList* const* lists, UINT numLists);
        void Signal();
        void Flush();                // Wait for all submitted work
        ID3D12CommandQueue* Get() const { return m_queue.Get(); }

    private:
        ComPtr<ID3D12CommandQueue> m_queue;
        ComPtr<ID3D12Fence>        m_fence;
        HANDLE                     m_fenceEvent = nullptr;
        UINT64                     m_fenceValue = 0;
    };
}
