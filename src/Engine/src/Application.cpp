#include <Engine/Application.hpp>

namespace Aurum
{
    Application::Application(HINSTANCE hInstance)
        : hInstance_(hInstance)
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
            {
                running_ = false;
                break;
            }

            timer_.Tick();
            double deltaTime = timer_.Tick();

            OnUpdate(deltaTime);

            // Clear with a calm blue background for now
            renderer_->Clear(0.05f, 0.05f, 0.25f);
            renderer_->Present();
        }

        Shutdown();
    }
}
