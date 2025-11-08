#include <Engine/Application.hpp>
#include <Engine/Event.hpp>

namespace Aurum
{
    Application::Application(HINSTANCE hInstance)
        : hInstance_(hInstance),
          input_(eventDispatcher_) // ✅ Integrate InputManager initialization
    {
        timeSystem_.Initialize(60.0); // Default 60 FPS cap
        running_ = true;
    }

    Application::~Application()
    {
        Shutdown();
    }

    // ------------------------------------------------------------
    void Application::Initialize()
    {
        Logger::Get().Log("Initializing Application...", LogLevel::Info);

        // --- Load runtime configuration ---
        runtimeConfig_.Load("config/engine_runtime.json");

        // --- Initialize time system with configured target FPS ---
        timeSystem_.Initialize(runtimeConfig_.GetTargetFPS());

        // --- Create the main application window ---
        window_ = std::make_unique<Window>(
            hInstance_,
            runtimeConfig_.GetWidth(),
            runtimeConfig_.GetHeight(),
            L"Aurum Engine Sandbox"
        );

        // --- Create the renderer ---
        renderer_ = std::make_unique<Renderer>(window_->GetHandle());

        // --- Attach this Application instance to the window for message forwarding ---
        SetWindowLongPtr(window_->GetHandle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

        // --- Publish initial resize event ---
        eventDispatcher_.Publish(WindowResizeEvent(
            runtimeConfig_.GetWidth(),
            runtimeConfig_.GetHeight()
        ));

        OnInitialize();
    }

    // ------------------------------------------------------------
    void Application::Shutdown()
    {
        if (!running_) return;
        running_ = false;

        Logger::Get().Log("Application shutting down...", LogLevel::Info);

        OnShutdown();

        renderer_.reset();
        window_.reset();

        Logger::Get().Log("Application shutdown complete.", LogLevel::Info);
    }

    // ------------------------------------------------------------
    void Application::Run()
    {
        Initialize();
        Logger::Get().Log("Application loop starting...", LogLevel::Info);

        MSG msg = {};
        timer_.Tick();    // Prime timer
        timeSystem_.Tick();

        while (running_)
        {
            // --- Windows Message Pump ---
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    Logger::Get().Log("WM_QUIT received — exiting main loop.", LogLevel::Info);
                    running_ = false;
                    break;
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            if (!running_) break;

            // --- Frame timing ---
            timeSystem_.Tick();
            const float dt = static_cast<float>(timeSystem_.GetDeltaTime());

            // --- Game / Engine update ---
            OnUpdate(dt);

            // --- Rendering ---
            if (renderer_)
            {
                renderer_->Clear(0.1f, 0.1f, 0.3f); // Default clear color
                renderer_->Present();
            }

            // --- Debug logging ---
            Logger::Get().Log(
                "Δt: " + std::to_string(dt) +
                "s | FPS: " + std::to_string(timeSystem_.GetFPS()),
                LogLevel::Debug
            );
        }

        Shutdown();
    }
}
