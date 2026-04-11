#include "sap_profiling/instrumentation_timer.h"
#include "sap_profiling/instrumentor.h"

#include <sap_core/types.h>
#include <thread>

namespace sap::prof {
    InstrumentationTimer::InstrumentationTimer(const char* name) :
        m_name(name), m_start_time(std::chrono::steady_clock::now()), m_stopped(false) {}

    InstrumentationTimer::~InstrumentationTimer() {
        if (!m_stopped)
            stop();
    }

    void InstrumentationTimer::stop() {
        auto end_tp = std::chrono::steady_clock::now();
        auto high_res_start = std::chrono::duration<f64, std::micro>{m_start_time.time_since_epoch()};
        auto elapsed_time = std::chrono::time_point_cast<std::chrono::microseconds>(end_tp).time_since_epoch() -
            std::chrono::time_point_cast<std::chrono::microseconds>(m_start_time).time_since_epoch();
        Instrumentor::get().write_profile({.name = m_name, .start = high_res_start, .duration = elapsed_time, .thread_id = std::this_thread::get_id()});
        m_stopped = true;
    }
} // namespace sap::prof
