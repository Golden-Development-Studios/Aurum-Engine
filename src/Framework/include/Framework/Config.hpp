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
        // Singleton Accessor
        static ConfigManager& Get()
        {
            static ConfigManager instance;
            return instance;
        }

        // -----------------------------
        // Load / Save JSON Config
        // -----------------------------
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

        // -----------------------------
        // Generic JSON Accessors
        // -----------------------------
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

        // -----------------------------
        // Strongly Typed Helpers
        // -----------------------------
        int GetInt(const std::string& key, int defaultValue = 0) const
        {
            return GetValue<int>(key, defaultValue);
        }

        float GetFloat(const std::string& key, float defaultValue = 0.0f) const
        {
            return GetValue<float>(key, defaultValue);
        }

        bool GetBool(const std::string& key, bool defaultValue = false) const
        {
            if (!data_.contains(key))
                return defaultValue;

            try
            {
                if (data_.at(key).is_boolean())
                    return data_.at(key).get<bool>();

                if (data_.at(key).is_number_integer())
                    return data_.at(key).get<int>() != 0;

                if (data_.at(key).is_string())
                {
                    std::string val = data_.at(key).get<std::string>();
                    return (val == "1" || val == "true" || val == "True" || val == "TRUE");
                }

                return defaultValue;
            }
            catch (...)
            {
                Logger::Get().Log("Config key parse failed for bool: " + key, LogLevel::Warning);
                return defaultValue;
            }
        }

        std::string GetString(const std::string& key, const std::string& defaultValue = "") const
        {
            return GetValue<std::string>(key, defaultValue);
        }

    private:
        // Private Constructor (Singleton)
        ConfigManager() = default;
        ~ConfigManager() = default;

        ConfigManager(const ConfigManager&) = delete;
        ConfigManager& operator=(const ConfigManager&) = delete;

        json data_;
        std::string filePath_;
    };
}
