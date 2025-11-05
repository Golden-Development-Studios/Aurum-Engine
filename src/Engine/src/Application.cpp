#include <Engine/Application.hpp>
#include <Engine/Event.hpp>

namespace Aurum
{
    Application::Application(HINSTANCE hInstance)
        : hInstance_(hInstance),
          input_(eventDispatcher_) // ✅ Integrate InputManager initialization
    {
    }

    Application::~Application()
    {
        Shutdown();
    }

    void Application::Initialize()
    {
        Logger::Get().Log("Initializing Application...", LogLevel::Info);

        window_ = std::make_unique<Window>(hInstance_, 1280, 720, L"Aurum Sandbox");
        renderer_ = std::make_unique<Renderer>(window_->GetHandle());

        // ✅ Attach this Application instance to the window for message forwarding
        SetWindowLongPtr(window_->GetHandle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

        // Example: publish a window resize event (simulated)
        eventDispatcher_.Publish(WindowResizeEvent(1280, 720));

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
            // Process Win32 messages (returns false if WM_QUIT received)
            if (!window_->ProcessMessages())
            {
                running_ = false;
                break;
            }

            // Update delta time
            double deltaTime = timer_.Tick();

            // User-defined update callback
            OnUpdate(static_cast<float>(deltaTime));

            // Example: Rendering workflow
            renderer_->Clear(0.05f, 0.05f, 0.25f);
            renderer_->Present();
        }

        Shutdown();
    }
}
