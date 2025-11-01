#include <windows.h>
#include "Engine/Window.hpp"
#include "Engine/Renderer.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    Aurum::Logger::Get().Log("Aurum Sandbox starting...", Aurum::LogLevel::Info);

    Aurum::Window window(hInstance, 1280, 720, L"Aurum Sandbox");
    Aurum::Renderer renderer(window.GetHandle());

    bool running = true;
    while (running && window.ProcessMessages())
    {
        renderer.Clear(0.1f, 0.1f, 0.3f);
        renderer.Present();
    }

    Aurum::Logger::Get().Log("Aurum Sandbox exiting...", Aurum::LogLevel::Info);
    return 0;
}
