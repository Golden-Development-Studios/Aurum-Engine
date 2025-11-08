#include <Windows.h>
#include <iostream>
#include <cmath>   // for sin/cos wave color variation
#include <Engine/Application.hpp>
#include <Engine/EventDispatcher.hpp>
#include <Engine/Event.hpp>
#include <Engine/DebugOverlay.hpp>

class SandboxApp : public Aurum::Application
{
public:
    using Aurum::Application::Application;

protected:
    void OnInitialize() override
    {
        Aurum::Logger::Get().Log("SandboxApp initialized.", Aurum::LogLevel::Info);

        // ✅ Access dispatcher through Application getter
        auto& dispatcher = GetEventDispatcher();

        // --------------------------------------------
        // Subscribe to window resize events
        // --------------------------------------------
        dispatcher.Subscribe<Aurum::WindowResizeEvent>(
            [](const Aurum::WindowResizeEvent& e)
            {
                Aurum::Logger::Get().Log("Received event: " + e.ToString(), Aurum::LogLevel::Info);
            }
        );

        // --------------------------------------------
        // Subscribe to keyboard events
        // --------------------------------------------
        dispatcher.Subscribe<Aurum::KeyPressedEvent>(
            [](const Aurum::KeyPressedEvent& e)
            {
                Aurum::Logger::Get().Log(e.ToString(), Aurum::LogLevel::Info);
            }
        );

        dispatcher.Subscribe<Aurum::KeyReleasedEvent>(
            [](const Aurum::KeyReleasedEvent& e)
            {
                Aurum::Logger::Get().Log(e.ToString(), Aurum::LogLevel::Info);
            }
        );

        // --------------------------------------------
        // Initialize Debug Overlay
        // --------------------------------------------
        overlay_.Initialize();

        // Simulate initial resize event
        dispatcher.Publish(Aurum::WindowResizeEvent(1920, 1080));
    }

    void OnUpdate(float dt) override
    {
        overlay_.Update(timeSystem_, runtimeConfig_.ShouldShowFPS());

        auto* renderer = GetRenderer();
        if (renderer)
        {
            // Animate background color using time-based sin waves
            double t = timeSystem_.GetTotalTime();
            float r = static_cast<float>((sin(t) * 0.5) + 0.5);
            float g = static_cast<float>((sin(t * 0.7) * 0.5) + 0.5);
            float b = static_cast<float>((sin(t * 1.3) * 0.5) + 0.5);

            renderer->Clear(r * 0.4f, g * 0.4f, b * 0.8f);
            renderer->Present();

            // --- Optional debug output ---
            Aurum::Logger::Get().Log(
                "Δt: " + std::to_string(dt) + "s | RGB(" +
                std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ")",
                Aurum::LogLevel::Debug
            );
        }
    }

    void OnShutdown() override
    {
        overlay_.Shutdown();
        Aurum::Logger::Get().Log("SandboxApp shutting down.", Aurum::LogLevel::Info);
    }

private:
    Aurum::DebugOverlay overlay_;
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
#if defined(_DEBUG)
    // --- Development console allocation ---
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    std::ios::sync_with_stdio();
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::wcout.clear();
    std::wclog.clear();
    std::wcerr.clear();

    std::cout << "=== Aurum Sandbox Debug Console ===" << std::endl;
#endif

    SandboxApp app(hInstance);
    app.Run();

#if defined(_DEBUG)
    std::cout << "\nPress Enter to close console..." << std::endl;
    std::cin.get();
    FreeConsole();
#endif

    return 0;
}
