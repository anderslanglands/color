#pragma once

#include "color/spectral_power_distribution.hpp"

#include <unordered_map>

namespace color {

struct Illuminant {
    enum class ID : int { D50, D55, D60, D65, E, P3 };

    static const SPD D65;

    static const std::unordered_map<ID, const SPD&> map;

    static auto get(ID id) -> const SPD& {
        return map.at(id);
    }
};

}
