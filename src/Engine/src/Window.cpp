// --- Aurum Engine Window Implementation ---
// Unicode + modern Win32 API version (DX12 compatible)

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <Engine/Window.hpp>
#include <Engine/Application.hpp>
#include <Engine/Input.hpp>
#include <Engine/Event.hpp>
#include <Framework/Logger.hpp>

namespace Aurum
{
    // ============================================================
    // Static Window Procedure
    // ============================================================
    LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Application* app = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        if (app)
        {
            // Forward input to InputManager
            app->GetInputManager().ProcessMessage(msg, wParam, lParam);
        }

        switch (msg)
        {
            case WM_CLOSE:
                DestroyWindow(hwnd);
                return 0;

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            default:
                return DefWindowProcW(hwnd, msg, wParam, lParam);
        }
    }

    // ============================================================
    // Constructor
    // ============================================================
    Window::Window(HINSTANCE hInstance, int width, int height, const std::wstring& title)
        : hInstance_(hInstance)
    {
        className_ = L"AurumWindowClass";

        WNDCLASSW wc = {};
        wc.lpfnWndProc   = WindowProc;
        wc.hInstance     = hInstance_;
        wc.lpszClassName = className_.c_str();
        wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

        if (!RegisterClassW(&wc))
        {
            Logger::Get().Log("❌ Failed to register window class.", LogLevel::Error);
            return;
        }

        RECT rect = { 0, 0, width, height };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        hwnd_ = CreateWindowExW(
            0,
            className_.c_str(),
            title.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            hInstance_,
            nullptr
        );

        if (!hwnd_)
        {
            DWORD errorCode = GetLastError();
            Logger::Get().Log("❌ Failed to create Win32 window. Error Code: " + std::to_string(errorCode), LogLevel::Error);
            return;
        }

        ShowWindow(hwnd_, SW_SHOW);
        UpdateWindow(hwnd_);

        Logger::Get().Log("✅ Window created successfully.", LogLevel::Info);
        MessageBoxW(hwnd_, L"Window successfully created.\nClick OK to start the engine loop.", L"Aurum Engine Debug", MB_OK | MB_ICONINFORMATION);

    }

    // ============================================================
    // Destructor
    // ============================================================
    Window::~Window()
    {
        if (hwnd_)
        {
            DestroyWindow(hwnd_);
            hwnd_ = nullptr;
        }

        if (!className_.empty())
        {
            UnregisterClassW(className_.c_str(), hInstance_);
        }
    }

    // ============================================================
    // Message Pump
    // ============================================================
    bool Window::ProcessMessages()
    {
        MSG msg = {};
        while (PeekMessageW(&msg, nullptr, 0u, 0u, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                Logger::Get().Log("ℹ️ WM_QUIT received — exiting.", LogLevel::Info);
                return false;
            }

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        return true;
    }

}
