#include <Framework/Logger.hpp>
#include <Framework/Config.hpp>

int main()
{
    using namespace Aurum;

    Logger::Get().SetLogFile("AurumLog.txt");
    Logger::Get().Log("Aurum Sandbox starting...", LogLevel::Info);

    ConfigManager& cfg = ConfigManager::Get();
    if (cfg.Load("settings.json"))
    {
        std::string title = cfg.GetValue<std::string>("window_title", "Untitled");
        int width = cfg.GetValue<int>("width", 800);
        int height = cfg.GetValue<int>("height", 600);
        bool fullscreen = cfg.GetValue<bool>("fullscreen", false);

        Logger::Get().Log("Loaded config:", LogLevel::Info);
        Logger::Get().Log("  Title: " + title);
        Logger::Get().Log("  Resolution: " + std::to_string(width) + "x" + std::to_string(height));
        Logger::Get().Log(std::string("  Fullscreen: ") + (fullscreen ? "true" : "false"));
    }

    cfg.SetValue("last_run_time", "2025-10-27 16:25");
    cfg.Save();  // overwrites the same file

    Logger::Get().Log("Aurum Sandbox exiting...", LogLevel::Info);
    return 0;
}
