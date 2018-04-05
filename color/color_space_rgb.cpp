#include "color/color_space_rgb.hpp"

namespace color {

const ColorSpaceRGB::Primaries
    ColorSpaceRGB::Primaries::itur_bt709(Chromaticity(0.64, 0.33),
                                         Chromaticity(0.30, 0.60),
                                         Chromaticity(0.15, 0.06));

const ColorSpaceRGB::WhitePoint
    ColorSpaceRGB::WhitePoint::D65(Chromaticity(0.3128f, 0.3290f),
                                   Illuminant::ID::D65);
const ColorSpaceRGB::WhitePoint
    ColorSpaceRGB::WhitePoint::D60(Chromaticity(0.32168f, 0.33767f),
                                   Illuminant::ID::D60);

const ColorSpaceRGB ColorSpaceRGB::ITUR_BT709(
    "ITU-R BT.709", ColorSpaceRGB::Primaries::itur_bt709,
    ColorSpaceRGB::WhitePoint::D65, OETF::rec709, EOTF::rec709);

const ColorSpaceRGB ColorSpaceRGB::ITUR_BT709_linear(
    "ITU-R BT.709 (linear)", ColorSpaceRGB::Primaries::itur_bt709,
    ColorSpaceRGB::WhitePoint::D65, OETF::linear, EOTF::linear);

const ColorSpaceRGB
    ColorSpaceRGB::ITUR_sRGB("sRGB", ColorSpaceRGB::Primaries::itur_bt709,
                             ColorSpaceRGB::WhitePoint::D65, OETF::sRGB,
                             EOTF::sRGB);

RGBf32 xyz_to_rgb(XYZ xyz, const ColorSpaceRGB& cs, bool ignore_transfer) {
    RGBf32 rgb;
    const M33f& m = cs.m_xyz_to_rgb;
    rgb.r = m[0][0] * xyz.x + m[0][1] * xyz.y + m[0][2] * xyz.z;
    rgb.g = m[1][0] * xyz.x + m[1][1] * xyz.y + m[1][2] * xyz.z;
    rgb.b = m[2][0] * xyz.x + m[2][1] * xyz.y + m[2][2] * xyz.z;
    if (ignore_transfer) {
        return rgb;
    } else {
        return cs.oetf(rgb);
    }
}
XYZ rgb_to_xyz(RGBf32 rgb, const ColorSpaceRGB& cs, bool ignore_transfer) {
    XYZ xyz;
    if (!ignore_transfer) {
        rgb = cs.eotf(rgb);
    }
    const M33f& m = cs.m_rgb_to_xyz;
    xyz.x = m[0][0] * rgb.r + m[0][1] * rgb.g + m[0][2] * rgb.b;
    xyz.y = m[1][0] * rgb.r + m[1][1] * rgb.g + m[1][2] * rgb.b;
    xyz.z = m[2][0] * rgb.r + m[2][1] * rgb.g + m[2][2] * rgb.b;
    return xyz;
}
}
