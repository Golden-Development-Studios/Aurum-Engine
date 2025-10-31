#pragma once
#include <cstddef>
#include <mutex>
#include <unordered_map>
#include <Framework/Logger.hpp>

namespace Aurum
{
    class MemoryTracker
    {
    public:
        static MemoryTracker& Get()
        {
            static MemoryTracker instance;
            return instance;
        }

        void RegisterAllocation(std::size_t size, const char* file, int line)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            totalAllocated_ += size;
            allocationCount_++;
            if constexpr (kVerbose)
            {
                Logger::Get().Log("Alloc " + std::to_string(size) + " bytes at " + file + ":" + std::to_string(line));
            }
        }

        void RegisterDeallocation(std::size_t size)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            totalFreed_ += size;
            deallocationCount_++;
        }

        void Report() const
        {
            Logger::Get().Log("=== Memory Report ===", LogLevel::Info);
            Logger::Get().Log("Allocated:   " + std::to_string(totalAllocated_) + " bytes", LogLevel::Info);
            Logger::Get().Log("Freed:       " + std::to_string(totalFreed_) + " bytes", LogLevel::Info);
            Logger::Get().Log("Allocations: " + std::to_string(allocationCount_), LogLevel::Info);
            Logger::Get().Log("Deallocs:    " + std::to_string(deallocationCount_), LogLevel::Info);
            Logger::Get().Log("=====================", LogLevel::Info);
        }

    private:
        MemoryTracker() = default;
        ~MemoryTracker() = default;

        MemoryTracker(const MemoryTracker&) = delete;
        MemoryTracker& operator=(const MemoryTracker&) = delete;

        mutable std::mutex mutex_;
        std::size_t totalAllocated_ = 0;
        std::size_t totalFreed_ = 0;
        std::size_t allocationCount_ = 0;
        std::size_t deallocationCount_ = 0;

        static constexpr bool kVerbose = false; // toggle detailed per-allocation logs
    };

    // Macros for tracking allocations (active only in debug)
    #if defined(_DEBUG) || !defined(NDEBUG)
        #define AURUM_NEW(T, ...) ([](){ \
            T* ptr = new T(__VA_ARGS__); \
            Aurum::MemoryTracker::Get().RegisterAllocation(sizeof(T), __FILE__, __LINE__); \
            return ptr; \
        }())

        #define AURUM_DELETE(ptr) do { \
            if(ptr){ \
                Aurum::MemoryTracker::Get().RegisterDeallocation(sizeof(*ptr)); \
                delete ptr; ptr = nullptr; \
            } \
        } while(0)
    #else
        #define AURUM_NEW(T, ...) new T(__VA_ARGS__)
        #define AURUM_DELETE(ptr) do { delete ptr; ptr = nullptr; } while(0)
    #endif
}
