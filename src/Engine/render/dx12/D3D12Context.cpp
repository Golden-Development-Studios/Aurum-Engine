#include "D3D12Context.hpp"
using namespace Aurum::Render::DX12;

// ------------------------------------------------------------
// Utility: Output to Visual Studio Debug Console
// ------------------------------------------------------------
static void OutputToDebugger(const std::string& s)
{
    OutputDebugStringA(s.c_str());
}

// ------------------------------------------------------------
// Logging helpers
// ------------------------------------------------------------
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

// ------------------------------------------------------------
// Helper: Convert wide strings (UTF-16) → UTF-8 safely
// ------------------------------------------------------------
static std::string ToUTF8(const std::wstring& wstr)
{
    if (wstr.empty()) return {};
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(sizeNeeded - 1, '\0'); // minus null terminator
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, str.data(), sizeNeeded, nullptr, nullptr);
    return str;
}

// ------------------------------------------------------------
// Utility: D3D Feature Level → String
// ------------------------------------------------------------
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

// ------------------------------------------------------------
// Initialization Sequence
// ------------------------------------------------------------
bool D3D12Context::Initialize(const ContextCreateDesc& desc)
{
    if (!CreateFactory(desc.enableDebugLayer))
        return false;
    if (!PickAdapter())
        return false;
    if (!CreateDevice())
        return false;

    // ------------------------------------------------------------
    // Stage 3.3: Initialize subsystems
    // ------------------------------------------------------------

    // Create Command Queue
    if (!m_commandQueue.Initialize(m_device.Get()))
        return false;

    // Create Swapchain (requires factory, device, queue, hwnd, size)
    if (!m_swapchain.Initialize(
            m_factory.Get(),
            m_device.Get(),
            m_commandQueue.Get(),
            desc.windowHandle,
            desc.width,
            desc.height))
        return false;

    // ------------------------------------------------------------
    // Summary log
    // ------------------------------------------------------------
    std::wstring fl = FeatureLevelToString(m_featureLevel);
    std::wstring gpu = m_adapterInfo.description;
    const double vramMB = static_cast<double>(m_adapterInfo.dedicatedVRAMBytes) / (1024.0 * 1024.0);

    LogInfo("Initialized D3D12 device. FeatureLevel=" + ToUTF8(fl) +
            ", Adapter=" + ToUTF8(gpu) +
            ", VRAM=" + std::to_string(static_cast<int>(vramMB)) + " MB");

    return true;
}


// ------------------------------------------------------------
// Create DXGI Factory (with optional debug layer)
// ------------------------------------------------------------
bool D3D12Context::CreateFactory(bool enableDebug)
{
    UINT dxgiFactoryFlags = 0;

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

// ------------------------------------------------------------
// Check Device Support Levels
// ------------------------------------------------------------
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

// ------------------------------------------------------------
// Adapter Selection (Hardware → WARP fallback)
// ------------------------------------------------------------
bool D3D12Context::PickAdapter()
{
    if (!m_factory)
        return false;

    ComPtr<IDXGIAdapter1> bestAdapter;
    D3D_FEATURE_LEVEL bestLevel = D3D_FEATURE_LEVEL_11_0;
    uint64_t bestVRAM = 0;
    bool found = false;

    // Prefer high-performance GPUs if available
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
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            D3D_FEATURE_LEVEL level;
            if (CheckDeviceSupport(adapter.Get(), level))
            {
                if (!found || (level > bestLevel) ||
                    (level == bestLevel && desc.DedicatedVideoMemory > bestVRAM))
                {
                    bestAdapter = adapter;
                    bestLevel = level;
                    bestVRAM = desc.DedicatedVideoMemory;
                    found = true;
                }
            }
        }
    }

    // Fallback: Enumerate all adapters if needed
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
                if (!found || (level > bestLevel) ||
                    (level == bestLevel && desc.DedicatedVideoMemory > bestVRAM))
                {
                    bestAdapter = adapter;
                    bestLevel = level;
                    bestVRAM = desc.DedicatedVideoMemory;
                    found = true;
                }
            }
        }
    }

    // Final fallback: WARP
    if (!found)
    {
        LogWarn("Falling back to WARP (software) adapter.");
        if (FAILED(m_factory->EnumWarpAdapter(IID_PPV_ARGS(&bestAdapter))))
        {
            LogError("Failed to acquire WARP adapter.");
            return false;
        }
        if (!CheckDeviceSupport(bestAdapter.Get(), bestLevel))
        {
            LogError("WARP adapter does not support required D3D feature levels.");
            return false;
        }
    }

    // Store final adapter info
    m_adapter = bestAdapter;
    m_featureLevel = bestLevel;

    DXGI_ADAPTER_DESC1 desc;
    m_adapter->GetDesc1(&desc);
    m_adapterInfo.description = desc.Description;
    m_adapterInfo.dedicatedVRAMBytes = desc.DedicatedVideoMemory;
    m_adapterInfo.isSoftware = (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0;

    // Log adapter info safely (UTF-8)
    LogInfo("Selected adapter: " + ToUTF8(m_adapterInfo.description));
    LogInfo("Feature level: " + ToUTF8(FeatureLevelToString(m_featureLevel)));
    LogInfo("VRAM (MB): " + std::to_string(static_cast<int>(m_adapterInfo.dedicatedVRAMBytes / (1024 * 1024))));

    return true;
}

// ------------------------------------------------------------
// Device Creation
// ------------------------------------------------------------
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
    // Enable GPU-based validation for debug builds
    ComPtr<ID3D12InfoQueue> infoQueue;
    if (SUCCEEDED(m_device.As(&infoQueue)) && infoQueue)
    {
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
    }
    LogInfo("D3D12 device created with InfoQueue in Debug.");
#endif

    return true;
}
