#pragma once

#include "color/math.hpp"

#include <spdlog/fmt/ostr.h>

#include <limits>

namespace color {

// Floating-point RGB type
template <typename T> struct RGBf {
    T r;
    T g;
    T b;

    using channel_type = T;

    RGBf(T v = 0.0f) : r(v), g(v), b(v) {}
    RGBf(T r, T g, T b) : r(r), g(g), b(b) {}

    RGBf(const RGBf&) = default;
    RGBf& operator=(const RGBf&) = default;
    RGBf(RGBf&&) = default;
    RGBf& operator=(RGBf&&) = default;

    // Conversion from different channel type
    template <typename U> RGBf(const RGBf<U>& other) {
        r = T(other.r);
        g = T(other.g);
        b = T(other.b);
    }

    RGBf<T> operator+(RGBf<T> o) const {
        RGBf<T> c(r + o.r, g + o.g, b + o.b);
        return c;
    }

    RGBf<T>& operator+=(RGBf<T> o) {
        r += o.r;
        g += o.g;
        b += o.b;
        return *this;
    }

    RGBf<T> operator-(RGBf<T> o) const {
        RGBf<T> c(r - o.r, g - o.g, b - o.b);
        return c;
    }

    RGBf<T>& operator-=(RGBf<T> o) {
        r -= o.r;
        g -= o.g;
        b -= o.b;
        return *this;
    }

    RGBf<T> operator*(RGBf<T> o) const {
        RGBf<T> c(r * o.r, g * o.g, b * o.b);
        return c;
    }

    RGBf<T>& operator*=(RGBf<T> o) {
        r *= o.r;
        g *= o.g;
        b *= o.b;
        return *this;
    }

    RGBf<T> operator/(RGBf<T> o) const {
        RGBf<T> c(r / o.r, g / o.g, b / o.b);
        return c;
    }

    RGBf<T>& operator/=(RGBf<T> o) {
        r /= o.r;
        g /= o.g;
        b /= o.b;
        return *this;
    }

    bool operator==(RGBf<T> o) { return r == o.r && g == o.g && b == o.b; }

    const T operator[](int idx) const noexcept {
        color_assert(idx >= 0 && idx < 3, "RGBf::operator[] out of bounds: {}", idx);
        return (&r)[idx];
    }

    T& operator[](int idx) noexcept {
        color_assert(idx >= 0 && idx < 3, "RGBf::operator[] out of bounds: {}", idx);
        return (&r)[idx];
    }
};

// Unsigned RGB type
template <typename T> struct RGBu {
    T r;
    T g;
    T b;

    using channel_type = T;

    RGBu(T v = 0.0f) : r(v), g(v), b(v) {}
    RGBu(T r, T g, T b) : r(r), g(g), b(b) {}

    RGBu(const RGBu&) = default;
    RGBu& operator=(const RGBu&) = default;
    RGBu(RGBu&&) = default;
    RGBu& operator=(RGBu&&) = default;

    RGBu<T> operator+(RGBu<T> o) const {
        RGBu<T> c(r + o.r, g + o.g, b + o.b);
        return c;
    }

    RGBu<T> operator+=(RGBu<T> o) {
        r += o.r;
        g += o.g;
        b += o.b;
        return *this;
    }

    RGBu<T> operator-(RGBu<T> o) const {
        RGBu<T> c(r - o.r, g - o.g, b - o.b);
        return c;
    }

    RGBu<T> operator-=(RGBu<T> o) {
        r -= o.r;
        g -= o.g;
        b -= o.b;
        return *this;
    }

    RGBu<T> operator*(RGBu<T> o) const {
        RGBu<T> c(r * o.r, g * o.g, b * o.b);
        return c;
    }

    RGBu<T> operator*=(RGBu<T> o) {
        r *= o.r;
        g *= o.g;
        b *= o.b;
        return *this;
    }

    RGBu<T> operator/(RGBu<T> o) const {
        RGBu<T> c(r / o.r, g / o.g, b / o.b);
        return c;
    }

    RGBu<T> operator/=(RGBu<T> o) {
        r /= o.r;
        g /= o.g;
        b /= o.b;
        return *this;
    }

    bool operator==(RGBu<T> o) { return r == o.r && g == o.g && b == o.b; }
};

using RGBf16 = RGBf<f16>;
using RGBf32 = RGBf<f32>;
using RGBf64 = RGBf<f64>;
using RGBu8 = RGBu<u8>;
using RGBu16 = RGBu<u16>;
using RGBu32 = RGBu<u32>;

// Convert from wider/narrower unsigned type via f32
template <typename T, typename U>
COLOR_FN_CONST inline auto rgb_cast(RGBu<U> other) -> RGBu<T> {
    auto t_max = f32(std::numeric_limits<T>::max());
    auto u_max = f32(std::numeric_limits<U>::max());
    auto r_ = f32(other.r) / u_max;
    auto g_ = f32(other.g) / u_max;
    auto b_ = f32(other.b) / u_max;
    RGBu<T> result{T(clamp(roundf(r_ * t_max), 0, t_max)),
                   T(clamp(roundf(g_ * t_max), 0, t_max)),
                   T(clamp(roundf(b_ * t_max), 0, t_max))};
    return result;
}

/// Convert from float to unsigned.
template <typename T, typename U>
COLOR_FN_CONST inline auto rgb_cast(RGBf<U> other) -> RGBu<T> {
    auto t_max = f32(std::numeric_limits<T>::max());
    RGBu<T> result{T(clamp(roundf(other.r * t_max), 0, t_max)),
                   T(clamp(roundf(other.g * t_max), 0, t_max)),
                   T(clamp(roundf(other.b * t_max), 0, t_max))};
    return result;
}

template <typename T>
COLOR_FN_CONST inline auto operator*(T f, RGBf<T> c) -> RGBf<T> {
    return c * f;
}

inline std::ostream& operator<<(std::ostream& os, RGBf32 c) {
    os << "(" << c.r << ", " << c.g << ", " << c.b << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, RGBu8 c) {
    os << "(" << std::setw(3) << int(c.r) << ", " << std::setw(3) << int(c.g)
       << ", " << std::setw(3) << int(c.b) << ")";
    return os;
}

COLOR_FN_CONST inline RGBf32 pow(RGBf32 c, f32 e) {
    return RGBf32(powf(c.r, e), powf(c.g, e), powf(c.b, e));
}

COLOR_FN_CONST inline RGBf32 clamp(RGBf32 c, RGBf32 mn, RGBf32 mx) {
    return RGBf32(
        clamp(c.r, mn.r, mx.r),
        clamp(c.g, mn.r, mx.g),
        clamp(c.b, mn.r, mx.b)
        );
}

COLOR_FN_CONST inline RGBf32 lerp(RGBf32 a, RGBf32 b, f32 t) { return (1 - t) * a + t * b; }

COLOR_FN_CONST inline RGBf32 max(RGBf32 c, f32 f) {
    c.r = std::max(c.r, f);
    c.g = std::max(c.g, f);
    c.b = std::max(c.b, f);
    return c;
}

COLOR_FN_CONST inline auto hmin(RGBf32 c) -> f32 {
    return std::min(c.r, std::min(c.g, c.b));
}

COLOR_FN_CONST inline auto hmax(RGBf32 c) -> f32 {
    return std::max(c.r, std::max(c.g, c.b));
}


COLOR_FN_CONST inline auto is_positive(RGBf32 c) -> bool { return hmax(c) > -1e-7f; }

COLOR_FN_CONST inline auto is_negative(RGBf32 c) -> bool { return !is_positive(c); }


COLOR_FN_CONST inline auto is_real(RGBf32 c) -> bool {
    return is_real(c.r) && is_real(c.g) && is_real(c.b);
}

COLOR_FN_CONST inline auto is_positive_real(RGBf32 c) -> bool { return is_real(c) && is_positive(c); }
}
