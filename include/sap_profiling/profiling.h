#pragma once
#include "sap_profiling/instrumentation_timer.h"
#include "sap_profiling/instrumentor.h"
#include "sap_profiling/utils.h"

#if SAP_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define FUNC_SIG __func__
#else
#define FUNC_SIG "FUNC_SIG unknown!"
#endif

#define PROFILE_BEGIN_SESSION(name, filepath) ::sap::prof::Instrumentor::get().begin_session(name, filepath)
#define PROFILE_END_SESSION() ::sap::prof::Instrumentor::get().end_session()
#define PROFILE_SCOPE_LINE2(name, line)                                                                                                    \
    constexpr auto fixedName##line = ::sap::prof::cleanup_output_string(name, "__cdecl ");                                                 \
    ::sap::prof::InstrumentationTimer timer##line(fixedName##line.Data)
#define PROFILE_SCOPE_LINE(name, line) PROFILE_SCOPE_LINE2(name, line)
#define PROFILE_SCOPE(name) PROFILE_SCOPE_LINE(name, __LINE__)
#define PROFILE_FUNCTION() PROFILE_SCOPE(FUNC_SIG)
#else
#define PROFILE_BEGIN_SESSION(name, filepath)
#define PROFILE_END_SESSION()
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif