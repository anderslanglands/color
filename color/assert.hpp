#pragma once

#include <spdlog/fmt/fmt.h>

#if !defined(NDEBUG)
#if !defined(COLOR_ASSERTIONS_NO_ABORT)
#define color_assert(e, ...)                                                   \
    if (!(e)) {                                                                \
        fmt::print(stderr, "[color] ASSERTION FAILED: \"{}\" at {}:{}\n", #e,  \
                   __FILE__, __LINE__);                                        \
        fmt::print(stderr, __VA_ARGS__);                                       \
        fmt::print(stderr, "\n");                                              \
        std::abort();                                                          \
    }
#else

#define color_assert(e, ...)                                                   \
    if (!(e)) {                                                                \
        fmt::print(stderr, "[color] ASSERTION FAILED: \"{}\" at {}:{}\n", #e,  \
                   __FILE__, __LINE__);                                        \
        fmt::print(stderr, __VA_ARGS__);                                       \
        fmt::print(stderr, "\n");                                              \
    }
#endif
#else

#define color_assert(...)

#endif
