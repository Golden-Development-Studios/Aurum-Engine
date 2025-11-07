#include "D3D12Context.hpp"

using namespace Aurum::Render::DX12;

static void OutputToDebugger(const std::string& s)
{
    OutputDebugStringA(s.c_str());
}

void D3D12Context::LogInfo(const std::string& msg)
{
    std::cout << "[DX12][INFO] " << msg << std::endl;
    OutputToDebugger("[DX12][INFO] " + msg + "\n");
}
void D3D12Context::LogWarn(const std::string& msg)
{
    std::cout << "[DX12][WARN] " << msg << std::endl;
    OutputToDebugger("[DX12][WARN] " + msg + "\n");
}
void D3D12Context::LogError(const std::string& msg)
{
    std::cerr << "[DX12][ERROR] " << msg << std::endl;
    OutputToDebugger("[DX12][ERROR] " + msg + "\n");
}

std::wstring D3D12Context::FeatureLevelToString(D3D_FEATURE_LEVEL fl)
{
    switch (fl)
    {
    case D3D_FEATURE_LEVEL_12_2: return L"12_2";
    case D3D_FEATURE_LEVEL_12_1: return L"12_1";
    case D3D_FEATURE_LEVEL_12_0: return L"12_0";
    case D3D_FEATURE_LEVEL_11_1: return L"11_1";
    case D3D_FEATURE_LEVEL_11_0: return L"11_0";
    default:                     return L"Unknown";
    }
}

bool D3D12Context::Initialize(const ContextCreateDesc& desc)
{
    if (!CreateFactory(desc.enableDebugLayer))
        return false;

    if (!PickAdapter())
        return false;

    if (!CreateDevice())
        return false;

    // Summary log
    std::wstring fl = FeatureLevelToString(m_featureLevel);
    std::wstring gpu = m_adapterInfo.description;
    const double vramMB = static_cast<double>(m_adapterInfo.dedicatedVRAMBytes) / (1024.0 * 1024.0);

    LogInfo(std::string("Initialized D3D12 device. FeatureLevel=") +
            std::string(std::begin(fl), std::end(fl)) +
            ", Adapter=" + std::string(gpu.begin(), gpu.end()) +
            ", VRAM=" + std::to_string(static_cast<int>(vramMB)) + " MB");

    return true;
}

bool D3D12Context::CreateFactory(bool enableDebug)
{
    UINT dxgiFactoryFlags = 0;

    // Enable debug layer when possible
#if defined(_DEBUG)
    enableDebug = true;
#endif

    if (enableDebug)
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            LogInfo("D3D12 debug layer enabled.");
        }
        else
        {
            LogWarn("Failed to acquire D3D12 debug interface. Continuing without debug layer.");
        }
    }

    HRESULT hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_factory));
    if (FAILED(hr) || !m_factory)
    {
        LogError("CreateDXGIFactory2 failed.");
        return false;
    }

    LogInfo("DXGI Factory created.");
    return true;
}

bool D3D12Context::CheckDeviceSupport(IDXGIAdapter1* adapter, D3D_FEATURE_LEVEL& outLevel)
{
    static const D3D_FEATURE_LEVEL kLevels[] = {
        D3D_FEATURE_LEVEL_12_2,
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };

    for (D3D_FEATURE_LEVEL level : kLevels)
    {
        ComPtr<ID3D12Device> testDevice;
        if (SUCCEEDED(D3D12CreateDevice(adapter, level, IID_PPV_ARGS(&testDevice))))
        {
            outLevel = level;
            return true;
        }
    }
    return false;
}

bool D3D12Context::PickAdapter()
{
    if (!m_factory) return false;

    // Prefer high-performance hardware adapters via IDXGIFactory6
    UINT index = 0;
    ComPtr<IDXGIAdapter1> bestAdapter;
    D3D_FEATURE_LEVEL bestLevel = D3D_FEATURE_LEVEL_11_0;
    uint64_t bestVRAM = 0;
    bool found = false;

    ComPtr<IDXGIFactory6> factory6 = m_factory;

    if (factory6)
    {
        for (UINT i = 0;; ++i)
        {
            ComPtr<IDXGIAdapter1> adapter;
            HRESULT hr = factory6->EnumAdapterByGpuPreference(
                i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
            if (hr == DXGI_ERROR_NOT_FOUND) break;
            if (FAILED(hr)) continue;

            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            // Skip software adapters here; consider later as fallback.
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            D3D_FEATURE_LEVEL level;
            if (CheckDeviceSupport(adapter.Get(), level))
            {
                // Pick the one with highest feature level, break ties with VRAM
                if (!found || (level > bestLevel) || (level == bestLevel && desc.DedicatedVideoMemory > bestVRAM))
                {
                    bestAdapter = adapter;
                    bestLevel = level;
                    bestVRAM = desc.DedicatedVideoMemory;
                    found = true;
                }
            }
        }
    }

    if (!found)
    {
        LogWarn("No suitable hardware adapter found via high-performance preference. Enumerating all adapters…");

        for (UINT i = 0;; ++i)
        {
            ComPtr<IDXGIAdapter1> adapter;
            if (m_factory->EnumAdapters1(i, &adapter) == DXGI_ERROR_NOT_FOUND)
                break;

            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            D3D_FEATURE_LEVEL level;
            if (CheckDeviceSupport(adapter.Get(), level))
            {
                if (!found || (level > bestLevel) || (level == bestLevel && desc.DedicatedVideoMemory > bestVRAM))
                {
                    bestAdapter = adapter;
                    bestLevel = level;
                    bestVRAM = desc.DedicatedVideoMemory;
                    found = true;
                }
            }
        }
    }

    // Fallback: WARP (software) if nothing else
    if (!found)
    {
        LogWarn("Falling back to WARP (software) adapter.");
        if (FAILED(m_factory->EnumWarpAdapter(IID_PPV_ARGS(&bestAdapter))))
        {
            LogError("Failed to acquire WARP adapter.");
            return false;
        }

        // Determine level supported by WARP
        if (!CheckDeviceSupport(bestAdapter.Get(), bestLevel))
        {
            LogError("WARP adapter does not support required D3D feature levels.");
            return false;
        }
    }

    m_adapter = bestAdapter;
    m_featureLevel = bestLevel;

    DXGI_ADAPTER_DESC1 desc;
    m_adapter->GetDesc1(&desc);

    m_adapterInfo.description = desc.Description;
    m_adapterInfo.dedicatedVRAMBytes = desc.DedicatedVideoMemory;
    m_adapterInfo.isSoftware = (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0;

    std::wstring fl = FeatureLevelToString(m_featureLevel);
    LogInfo("Selected adapter: " + std::string(m_adapterInfo.description.begin(), m_adapterInfo.description.end()));
    LogInfo("Feature level: " + std::string(fl.begin(), fl.end()));
    LogInfo("VRAM (MB): " + std::to_string(static_cast<int>(m_adapterInfo.dedicatedVRAMBytes / (1024 * 1024))));

    return true;
}

bool D3D12Context::CreateDevice()
{
    if (!m_adapter)
    {
        LogError("CreateDevice called without a valid adapter.");
        return false;
    }

    HRESULT hr = D3D12CreateDevice(m_adapter.Get(), m_featureLevel, IID_PPV_ARGS(&m_device));
    if (FAILED(hr) || !m_device)
    {
        LogError("D3D12CreateDevice failed on selected adapter.");
        return false;
    }

#if defined(_DEBUG)
    // Enable GPU-based validation if desired (optional; can be expensive)
    ComPtr<ID3D12InfoQueue> infoQueue;
    if (SUCCEEDED(m_device.As(&infoQueue)) && infoQueue)
    {
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        // Filter noisy messages here if needed.
    }
    LogInfo("D3D12 device created with InfoQueue in Debug.");
#endif

    return true;
}
