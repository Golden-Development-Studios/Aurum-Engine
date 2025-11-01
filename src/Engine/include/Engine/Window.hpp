#pragma once
#include <windows.h>
#include <string>
#include <Framework/Logger.hpp>

namespace Aurum
{
    class Window
    {
    public:
        Window(HINSTANCE hInstance, int width, int height, const std::wstring& title);
        ~Window();

        HWND GetHandle() const { return hwnd_; }
        bool ProcessMessages();

    private:
        HWND hwnd_ = nullptr;
        HINSTANCE hInstance_ = nullptr;
        std::wstring className_ = L"AurumWindowClass";

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}
