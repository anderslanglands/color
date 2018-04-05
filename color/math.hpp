#pragma once

#include "color/platform.hpp"
#include "color/types.hpp"

#include <cmath>

namespace color {

COLOR_FN_CONST inline auto clamp(f32 x, f32 a, f32 b) -> f32 {
    return std::max(a, std::min(x, b));
}

COLOR_FN_CONST inline auto lerp(f32 a, f32 b, f32 t) -> f32 {
    return (1 - t) * a + t * b;
}

COLOR_FN_CONST inline auto is_real(float f) -> bool { return std::isfinite(f); }
}
