#include <Engine/Window.hpp>
#include <Engine/Renderer.hpp>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    Aurum::Window window(hInstance, 1280, 720, L"Aurum Engine - Sandbox");
    Aurum::Renderer renderer(window.GetHandle());

    while (window.ProcessMessages())
    {
        renderer.Clear(0.1f, 0.1f, 0.4f);
        renderer.Present();
    }

    return 0;
}
