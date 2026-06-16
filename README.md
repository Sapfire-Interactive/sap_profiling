# sap_profiling

Instrumentation profiler for C++20. Records scoped timings per thread and writes them as Chrome Tracing JSON, viewable in [Perfetto](https://ui.perfetto.dev) or `chrome://tracing`.

Part of the [Sapfire](https://github.com/Sapfire-Interactive) library ecosystem.

## How it works

`Instrumentor` is a thread-safe singleton that manages a profiling session. `InstrumentationTimer` is a RAII guard: it records a `std::chrono::steady_clock` timestamp on construction and writes a `ProfileResult` (name, start, duration, thread ID) to the output stream on destruction.

Results are written in the Chrome Tracing `{"traceEvents": [...]}` format. Each event carries a thread ID, enabling per-thread timeline views in Perfetto.

## Usage

```cpp
#include <sap_profiling/profiling.h>

// Start a session — opens results.json (or a path of your choice)
sap::prof::Instrumentor::get().begin_session("MyApp");

{
    SAP_PROFILE_SCOPE("expensive_work");
    do_expensive_work();
} // timer fires here, writes the event

sap::prof::Instrumentor::get().end_session();
```

Open `results.json` in [Perfetto UI](https://ui.perfetto.dev) or `chrome://tracing` to view the flame graph.

## Build

Requires CMake 3.20+, C++20, and [sap_core](https://github.com/Sapfire-Interactive/sap_core).

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
