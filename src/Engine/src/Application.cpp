#include <Engine/Application.hpp>
#include <Engine/Event.hpp>

namespace Aurum
{
    Application::Application(HINSTANCE hInstance)
        : hInstance_(hInstance),
          input_(eventDispatcher_) // ✅ Integrate InputManager initialization
    {
        timeSystem_.Initialize(60.0); // Target 60 FPS Cap
    }

    Application::~Application()
    {
        Shutdown();
    }

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

        // --- Dispatch a resize event to start things properly ---
        eventDispatcher_.Publish(WindowResizeEvent(
            runtimeConfig_.GetWidth(),
            runtimeConfig_.GetHeight()
        ));

        OnInitialize();
    }


    void Application::Shutdown()
    {
        if (!running_) return;
        running_ = false;
        OnShutdown();

        renderer_.reset();
        window_.reset();

        Logger::Get().Log("Application shutdown complete.", LogLevel::Info);
    }

    void Application::Run()
    {
        Initialize();
        Logger::Get().Log("Application loop starting...", LogLevel::Info);

        timer_.Tick(); // initializes frame timing

        while (running_)
        {
            if (!window_->ProcessMessages())
                break;

            // ---- Update Timming ---
            timeSystem_.Tick();
            float dt = static_cast<float>(timeSystem_.GetDeltaTime());

            // ----- Game/Engine Update ----
            OnUpdate(dt);

            // ----- Rendering ----
            renderer_->Clear(0.1f, 0.1f, 0.3f);
            renderer_->Present();

            // --- Debug Logging ----
            Logger::Get().Log(
                "Δt: " + std::to_string(dt) +
                "s | FPS: " + std::to_string(timeSystem_.GetFPS()),
                LogLevel::Debug
    
            );
        }

        Shutdown();
    }
}
