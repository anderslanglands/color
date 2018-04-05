#pragma once

#include "color/spectral_power_distribution.hpp"

#include <unordered_map>

namespace color {

struct CMF {
    enum class ID : int {
        CIE_1931_2degree = 0,
        CIE_1964_10degree,
        CIE_2012_2degree,
        CIE_2012_10degree
    };

    CMF(const SPD& x_bar, const SPD& y_bar, const SPD& z_bar)
        : x_bar(x_bar), y_bar(y_bar), z_bar(z_bar) {}


    static const CMF& get(ID cmf) {
        return _cmf_map.at(cmf);
    }
    
    const SPD& x_bar;
    const SPD& y_bar;
    const SPD& z_bar;

    static const CMF CIE_1931_2degree;
    static const CMF CIE_1964_10degree;
    static const CMF CIE_2012_2degree;
    static const CMF CIE_2012_10degree;

    static const SPD CIE_1931_2_x;
    static const SPD CIE_1931_2_y;
    static const SPD CIE_1931_2_z;
    static const SPD CIE_1964_10_x;
    static const SPD CIE_1964_10_y;
    static const SPD CIE_1964_10_z;
    static const SPD CIE_2012_2_x;
    static const SPD CIE_2012_2_y;
    static const SPD CIE_2012_2_z;
    static const SPD CIE_2012_10_x;
    static const SPD CIE_2012_10_y;
    static const SPD CIE_2012_10_z;

private:
    static const std::unordered_map<ID, const CMF&> _cmf_map;
};
}
