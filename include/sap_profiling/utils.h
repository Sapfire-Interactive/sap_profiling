#pragma once

#include <sap_core/types.h>

namespace sap::prof {
    template <stl::size_t N>
    struct ChangeResult {
        char data[N];
    };

    template <stl::size_t N, stl::size_t K>
    constexpr auto cleanup_output_string(const char (&expr)[N], const char (&remove)[K]) {
        ChangeResult<N> result = {};
        size_t src_index = 0;
        size_t dst_index = 0;
        while (src_index < N) {
            size_t match_index = 0;
            while (match_index < K - 1 && src_index + match_index < N - 1 && expr[src_index + match_index] == remove[match_index])
                match_index++;
            if (match_index == K - 1)
                src_index += match_index;
            result.data[dst_index++] = expr[src_index] == '"' ? '\'' : expr[src_index];
            src_index++;
        }
        return result;
    }
} // namespace sap::prof