#pragma once

// Aurum Engine - D3D12 Context
// Creates DXGI factory, selects a hardware adapter, and initializes an ID3D12Device.

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>

namespace Aurum::Render::DX12
{
    using Microsoft::WRL::ComPtr;

    struct AdapterInfo
    {
        std::wstring description;
        uint64_t     dedicatedVRAMBytes = 0;
        bool         isSoftware         = false;
    };

    struct ContextCreateDesc
    {
        bool enableDebugLayer = false; // Enabled by default in Debug builds via Renderer.
    };

    class D3D12Context
    {
    public:
        D3D12Context() = default;
        ~D3D12Context() = default;

        bool Initialize(const ContextCreateDesc& desc);

        // Accessors
        IDXGIFactory6*    GetFactory() const    { return m_factory.Get(); }
        IDXGIAdapter1*    GetAdapter() const    { return m_adapter.Get(); }
        ID3D12Device*     GetDevice()  const    { return m_device.Get(); }
        D3D_FEATURE_LEVEL GetFeatureLevel() const { return m_featureLevel; }
        const AdapterInfo& GetAdapterInfo() const { return m_adapterInfo; }

    private:
        bool CreateFactory(bool enableDebug);
        bool PickAdapter();
        bool CreateDevice();

        // Helpers
        static bool CheckDeviceSupport(IDXGIAdapter1* adapter, D3D_FEATURE_LEVEL& outLevel);
        static std::wstring FeatureLevelToString(D3D_FEATURE_LEVEL fl);
        static void LogInfo(const std::string& msg);
        static void LogWarn(const std::string& msg);
        static void LogError(const std::string& msg);

    private:
        ComPtr<IDXGIFactory6> m_factory;
        ComPtr<IDXGIAdapter1> m_adapter;
        ComPtr<ID3D12Device>  m_device;

        D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_1;
        AdapterInfo       m_adapterInfo{};
    };
} // namespace Aurum::Render::DX12
