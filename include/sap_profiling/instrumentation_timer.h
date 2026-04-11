#pragma once

#include <chrono>

namespace sap::prof {
    class InstrumentationTimer {
    public:
        InstrumentationTimer(const char* name);
        ~InstrumentationTimer();

        void stop();
    private:
        const char* m_name;
        std::chrono::time_point<std::chrono::steady_clock> m_start_time;
        bool m_stopped;
    };
} // namespace sap::prof