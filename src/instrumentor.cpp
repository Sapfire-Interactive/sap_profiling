#include "sap_profiling/instrumentor.h"

#include <sap_core/log.h>
#include <sstream>

namespace sap::prof {
    Instrumentor& Instrumentor::get() {
        static Instrumentor instr;
        return instr;
    }

    void Instrumentor::begin_session(const stl::string_view name, const stl::string_view filepath) {
        stl::lock_guard<stl::mutex> lock{m_mutex};
        if (m_session) {
            log::error("Instrumentor::begin_session({}) when another session ({}) is already open, will end that session.", name,
                       m_session->name);
            end_session_no_lock();
        }
        m_output_stream.open(name.data());
        if (!m_output_stream.is_open()) {
            log::error("Failed to open file at path {}.", name);
            return;
        }

        m_session = stl::make_unique<Session>(name);
        write_header();
    }

    void Instrumentor::end_session() {
        stl::lock_guard<stl::mutex> lock{m_mutex};
        end_session_no_lock();
    }

    void Instrumentor::write_profile(const ProfileResult& profile_result) {
        std::stringstream json;
        json << std::setprecision(3) << std::fixed;
        json << ",{";
        json << "\"cat\":\"function\",";
        json << "\"dur\":" << profile_result.duration.count() << ',';
        json << "\"name\":\"" << profile_result.name << "\",";
        json << "\"ph\":\"X\",";
        json << "\"pid\":0,";
        json << "\"tid\":" << profile_result.thread_id << ",";
        json << "\"ts\":" << profile_result.start.count();
        json << "}";
        stl::lock_guard<stl::mutex> lock{m_mutex};
        if (m_session) {
            m_output_stream << json.str();
            m_output_stream.flush();
        }
    }

    Instrumentor::Instrumentor() : m_session(nullptr) {}

    Instrumentor::~Instrumentor() { end_session(); }

    void Instrumentor::end_session_no_lock() {
        if (m_session) {
            write_footer();
            m_output_stream.close();
            m_session.reset();
        }
    }

    void Instrumentor::write_header() {
        m_output_stream << "{\"otherData\": {},\"traceEvents\":[{}";
        m_output_stream.flush();
    }

    void Instrumentor::write_footer() {
        m_output_stream << "]}";
        m_output_stream.flush();
    }

} // namespace sap::prof