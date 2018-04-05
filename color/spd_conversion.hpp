#pragma once

#include "color/spectral_power_distribution.hpp"
#include "color/cmf.hpp"
#include "color/rgb.hpp"
#include "color/color_space_rgb.hpp"

namespace color {

auto spd_to_xyz(const SPD& spd, const CMF& cmf, const SPD& illuminant) -> XYZ;
auto spd_to_xyz(const SPD& spd, const CMF& cmf) -> XYZ;
auto spd_to_xyz(float wavelength, float value, const CMF& cmf) -> XYZ;
auto spd_to_rgb(const SPD& spd, const ColorSpaceRGB& cs) -> RGBf32;

}
