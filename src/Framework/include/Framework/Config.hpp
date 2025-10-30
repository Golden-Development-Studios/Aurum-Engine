#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <thirdparty/nlohmann/json.hpp>
#include <Framework/Logger.hpp>

namespace Aurum
{
    using json = nlohmann::json;

    class ConfigManager
    {
    public:
        static ConfigManager& Get()
        {
            static ConfigManager instance;
            return instance;
        }

        bool Load(const std::string& path)
        {
            std::ifstream file(path);
            if (!file.is_open())
            {
                Logger::Get().Log("Failed to open config file: " + path, LogLevel::Error);
                return false;
            }

            try
            {
                file >> data_;
                filePath_ = path;
                Logger::Get().Log("Config loaded successfully: " + path, LogLevel::Info);
                return true;
            }
            catch (const std::exception& e)
            {
                Logger::Get().Log(std::string("Config parse error: ") + e.what(), LogLevel::Error);
                return false;
            }
        }

        bool Save(const std::string& path = "")
        {
            std::string outPath = path.empty() ? filePath_ : path;
            if (outPath.empty())
            {
                Logger::Get().Log("Config save failed: No file path specified.", LogLevel::Error);
                return false;
            }

            std::ofstream file(outPath);
            if (!file.is_open())
            {
                Logger::Get().Log("Failed to open config for writing: " + outPath, LogLevel::Error);
                return false;
            }

            file << std::setw(4) << data_;
            Logger::Get().Log("Config saved: " + outPath, LogLevel::Info);
            return true;
        }

        template<typename T>
        T GetValue(const std::string& key, const T& defaultValue = T{}) const
        {
            if (!data_.contains(key))
                return defaultValue;
            try
            {
                return data_.at(key).get<T>();
            }
            catch (...)
            {
                Logger::Get().Log("Config key type mismatch: " + key, LogLevel::Warning);
                return defaultValue;
            }
        }

        template<typename T>
        void SetValue(const std::string& key, const T& value)
        {
            data_[key] = value;
        }

    private:
        ConfigManager() = default;
        ~ConfigManager() = default;

        ConfigManager(const ConfigManager&) = delete;
        ConfigManager& operator=(const ConfigManager&) = delete;

        json data_;
        std::string filePath_;
    };
}
