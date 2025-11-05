// --- Aurum Engine Window Implementation ---
// Unicode + modern Win32 API version

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
#include <Framework/Logger.hpp>  // Assuming you have a logger utility

namespace Aurum
{
    Window::Window(HINSTANCE hInstance, int width, int height, const std::wstring& title)
        : hInstance_(hInstance)
    {
        // Define a Win32 window class (wide char version)
        WNDCLASSW wc = {};
        wc.lpfnWndProc   = WindowProc;
        wc.hInstance     = hInstance_;
        wc.lpszClassName = className_.c_str();
        wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

        RegisterClassW(&wc);

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
            Logger::Get().Log("Failed to create Win32 window.", LogLevel::Error);
            return;
        }

        // Store a pointer to the application instance for message forwarding
        // You can later set this externally using SetWindowLongPtr.
        // Typically: SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)&Application::Get());

        ShowWindow(hwnd_, SW_SHOW);
        UpdateWindow(hwnd_);
        Logger::Get().Log("Window created successfully.", LogLevel::Info);
    }

    Window::~Window()
    {
        if (hwnd_)
            DestroyWindow(hwnd_);
        UnregisterClassW(className_.c_str(), hInstance_);
    }

    bool Window::ProcessMessages()
    {
        MSG msg = {};
        while (PeekMessageW(&msg, nullptr, 0u, 0u, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return false;

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        return true;
    }

    // ------------------------------------------------------------
    // Unified WindowProc - integrates input + event forwarding
    // ------------------------------------------------------------
    LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // Attempt to retrieve Application instance (set via SetWindowLongPtr)
        Application* app = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        if (app)
        {
            // Forward input to the InputManager for centralized handling
            app->GetInputManager().ProcessMessage(msg, wParam, lParam);
        }

        switch (msg)
        {
            case WM_CLOSE:
                PostQuitMessage(0);
                return 0;

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            default:
                return DefWindowProcW(hwnd, msg, wParam, lParam);
        }
    }
}
