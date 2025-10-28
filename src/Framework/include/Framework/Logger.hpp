#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

namespace Aurum
{
    enum class LogLevel
    {
        Info,
        Warning,
        Error,
        Debug
    };

    class Logger
    {
    public:
        static Logger& Get()
        {
            static Logger instance;
            return instance;
        }

        void SetLogFile(const std::string& filename)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            file_.open(filename, std::ios::out | std::ios::app);
        }

        void Log(const std::string& message, LogLevel level = LogLevel::Info)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            const std::string prefix = "[" + Timestamp() + "] " + LevelToString(level) + ": ";

            // Console output
            std::cout << prefix << message << std::endl;

            // File output
            if (file_.is_open())
                file_ << prefix << message << std::endl;
        }

    private:
        Logger() = default;
        ~Logger() = default;

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        std::string Timestamp()
        {
            using namespace std::chrono;
            auto now = system_clock::now();
            auto time = system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time), "%H:%M:%S");
            return ss.str();
        }

        std::string LevelToString(LogLevel level)
        {
            switch (level)
            {
                case LogLevel::Info: return "[INFO]";
                case LogLevel::Warning: return "[WARN]";
                case LogLevel::Error: return "[ERROR]";
                case LogLevel::Debug: return "[DEBUG]";
                default: return "[UNKNOWN]";
            }
        }

        std::ofstream file_;
        std::mutex mutex_;
    };
}
