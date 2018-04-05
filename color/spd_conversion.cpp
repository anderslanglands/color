#include "color/spd_conversion.hpp"

namespace color {
auto spd_to_xyz(const SPD& spd, const CMF& cmf, const SPD& illuminant) -> XYZ {
    // limit our calculation to the smallest range
    auto lambda_start =
        std::max(spd.start(), std::max(cmf.x_bar.start(), illuminant.start()));
    auto lambda_end =
        std::min(spd.end(), std::min(cmf.x_bar.end(), illuminant.end()));

    size_t idx_start = 0;
    while (spd._wavelengths[idx_start] < lambda_start) {
        idx_start++;
    }

    size_t idx_end = 0;
    while (spd._wavelengths[idx_end] < lambda_end &&
           idx_end < spd._wavelengths.size()) {
        idx_end++;
    }

    XYZ xyz(0.0f);
    float N = 0.0f;
    for (size_t i = idx_start; i < idx_end; ++i) {
        float l = spd._wavelengths[i];
        float Me = spd._values[i] * illuminant.value(l);
        xyz.x += Me * cmf.x_bar.value(l);
        xyz.y += Me * cmf.y_bar.value(l);
        xyz.z += Me * cmf.z_bar.value(l);
        N += cmf.y_bar.value(l) * illuminant.value(l);
    }

    return xyz / N;
}

auto spd_to_xyz(const SPD& spd, const CMF& cmf) -> XYZ {
    // limit our calculation to the smallest range
    auto lambda_start = std::max(spd.start(), cmf.x_bar.start());
    auto lambda_end = std::min(spd.end(), cmf.x_bar.end());

    size_t idx_start = 0;
    while (spd._wavelengths[idx_start] < lambda_start) {
        idx_start++;
    }

    size_t idx_end = 0;
    while (spd._wavelengths[idx_end] < lambda_end &&
           idx_end < spd._wavelengths.size()) {
        idx_end++;
    }

    XYZ xyz(0.0f);
    for (size_t i = idx_start; i < idx_end; ++i) {
        xyz.x += spd._values[i] * cmf.x_bar.value(spd._wavelengths[i]);
        xyz.y += spd._values[i] * cmf.y_bar.value(spd._wavelengths[i]);
        xyz.z += spd._values[i] * cmf.z_bar.value(spd._wavelengths[i]);
    }

    return xyz;
}

auto spd_to_xyz(float wavelength, float value, const CMF& cmf) -> XYZ {
    XYZ xyz;
    xyz.x = value * cmf.x_bar.value(wavelength);
    xyz.y = value * cmf.y_bar.value(wavelength);
    xyz.z = value * cmf.z_bar.value(wavelength);
    return xyz;
}

auto spd_to_xyz(float wavelength, float value, const CMF& cmf,
                const SPD& illuminant) -> XYZ {
    XYZ xyz;
    xyz.x = value * cmf.x_bar.value(wavelength) * illuminant.value(wavelength);
    xyz.y = value * cmf.y_bar.value(wavelength) * illuminant.value(wavelength);
    xyz.z = value * cmf.z_bar.value(wavelength) * illuminant.value(wavelength);
    return xyz / (cmf.y_bar.value(wavelength) * illuminant.value(wavelength));
}

auto spd_to_rgb(const SPD& spd, const ColorSpaceRGB& cs) -> RGBf32 {
    auto xyz =
        spd_to_xyz(spd, cs.cmf, Illuminant::get(cs.white_point.illuminant));
    return xyz_to_rgb(xyz, cs);
}
}
