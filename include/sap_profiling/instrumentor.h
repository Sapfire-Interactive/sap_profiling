#pragma once

#include <chrono>
#include <fstream>
#include <sap_core/stl/string.h>
#include <sap_core/types.h>
#include <sap_core/stl/unique_ptr.h>

namespace sap::prof {

    struct ProfileResult {
        stl::string name;
        std::chrono::duration<f64, std::micro> start;
        std::chrono::microseconds duration;
        stl::thread::id thread_id;
    };

    // This exists so we don't have to check whether the file is open or not
    // generally cheaper than caching string + bool (probably - doesn't matter because profiling builds anyway)
    struct Session {
        stl::string name;
    };

    class Instrumentor {
    public:
        Instrumentor(const Instrumentor&) = delete;
        Instrumentor& operator=(const Instrumentor&) = delete;
        static Instrumentor& get();
        void begin_session(const stl::string_view name, const stl::string_view filepath = "results.json");
        void end_session();
        void write_profile(const ProfileResult& profile_result);

    private:
        Instrumentor();
        ~Instrumentor();
        void end_session_no_lock();
        void write_header();
        void write_footer();

    private:
        stl::mutex m_mutex;
        std::ofstream m_output_stream;
        stl::unique_ptr<Session> m_session{nullptr};
    };
} // namespace sap::prof