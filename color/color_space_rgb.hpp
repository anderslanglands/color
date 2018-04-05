#pragma once

#include "color/cmf.hpp"
#include "color/illuminant.hpp"
#include "color/rgb.hpp"

namespace color {

using Chromaticity = V2f;

namespace OETF {
COLOR_FN_CONST inline auto linear(RGBf32 c) -> RGBf32 { return c; }
COLOR_FN_CONST inline auto sRGBf(float f) -> float {
    if (f <= 0.0031308f) {
        return 12.92f * f;
    } else {
        return (1.0f + 0.055f) * powf(f, 1.0f / 2.4f) - 0.055f;
    }
}

COLOR_FN_CONST inline auto sRGB(RGBf32 c) -> RGBf32 {
    return RGBf32(sRGBf(c.r), sRGBf(c.g), sRGBf(c.b));
}

COLOR_FN_CONST inline auto rec709f(float f) -> float {
    if (f <= 0.018f) {
        return f * 4.5f;
    } else {
        return 1.099f * powf(f, 0.45f) - 0.099f;
    }
}
COLOR_FN_CONST inline auto rec709(RGBf32 c) -> RGBf32 {
    return RGBf32(rec709f(c.r), rec709f(c.g), rec709f(c.b));
}
}

namespace EOTF {
COLOR_FN_CONST inline auto linear(RGBf32 c) -> RGBf32 { return c; }
COLOR_FN_CONST inline auto sRGBf(float f) -> float {
    if (f <= 0.040449936f) {
        return f / 12.92f;
    } else {
        return powf((f + 0.055f) / 1.055f, 2.4f);
    }
}
COLOR_FN_CONST inline auto sRGB(RGBf32 c) -> RGBf32 {
    return RGBf32(sRGBf(c.r), sRGBf(c.g), sRGBf(c.b));
}
COLOR_FN_CONST inline auto rec709f(float f) -> float {
    if (f <= 0.018f * 4.5f) {
        return f / 4.5f;
    } else {
        return powf((f + 0.099f) / 1.099f, 1.0f / 0.45f);
    }
}
COLOR_FN_CONST inline auto rec709(RGBf32 c) -> RGBf32 {
    return RGBf32(rec709f(c.r), rec709f(c.g), rec709f(c.b));
}
}

struct ColorSpaceRGB {
    struct Primaries {
        Chromaticity red;
        Chromaticity green;
        Chromaticity blue;
        Primaries(Chromaticity red, Chromaticity green, Chromaticity blue)
            : red(std::move(red)), green(std::move(green)),
              blue(std::move(blue)) {}

        static const Primaries itur_bt709;
    };

    struct WhitePoint {
        Chromaticity xy;
        Illuminant::ID illuminant;
        WhitePoint(Chromaticity xy, Illuminant::ID illuminant)
            : xy(std::move(xy)), illuminant(std::move(illuminant)) {}

        static const WhitePoint D60;
        static const WhitePoint D65;
    };

    std::string name;
    Primaries primaries;
    WhitePoint white_point;

    using TransferFunction = std::function<RGBf32(RGBf32)>;

    TransferFunction oetf;
    TransferFunction eotf;

    const CMF& cmf;

    M33f m_xyz_to_rgb;
    M33f m_rgb_to_xyz;

    ColorSpaceRGB(std::string name, Primaries primaries, WhitePoint white_point,
                  TransferFunction oetf = OETF::linear,
                  TransferFunction eotf = EOTF::linear,
                  const CMF& cmf = CMF::CIE_1931_2degree)
        : name(std::move(name)), primaries(std::move(primaries)),
          white_point(std::move(white_point)), oetf(std::move(oetf)),
          eotf(std::move(eotf)), cmf(cmf) {
        _initialize_matrix();
    }

    static const ColorSpaceRGB ITUR_BT709;
    static const ColorSpaceRGB ITUR_BT709_linear;
    static const ColorSpaceRGB ITUR_sRGB;

private:
    void _initialize_matrix() {
        float xr, yr, zr, xg, yg, zg, xb, yb, zb;
        float xw, yw, zw;
        float rx, ry, rz, gx, gy, gz, bx, by, bz;
        float rw, gw, bw;

        xr = primaries.red.x;
        yr = primaries.red.y;
        zr = 1 - (xr + yr);
        xg = primaries.green.x;
        yg = primaries.green.y;
        zg = 1 - (xg + yg);
        xb = primaries.blue.x;
        yb = primaries.blue.y;
        zb = 1 - (xb + yb);

        xw = white_point.xy.x;
        yw = white_point.xy.y;
        zw = 1 - (xw + yw);

        // xyz -> rgb matrix, before scaling to white
        rx = (yg * zb) - (yb * zg);
        ry = (xb * zg) - (xg * zb);
        rz = (xg * yb) - (xb * yg);
        gx = (yb * zr) - (yr * zb);
        gy = (xr * zb) - (xb * zr);
        gz = (xb * yr) - (xr * yb);
        bx = (yr * zg) - (yg * zr);
        by = (xg * zr) - (xr * zg);
        bz = (xr * yg) - (xg * yr);

        // White scaling factors.
        // Dividing by yw scales the white luminance to unity, as conventional
        rw = ((rx * xw) + (ry * yw) + (rz * zw)) / yw;
        gw = ((gx * xw) + (gy * yw) + (gz * zw)) / yw;
        bw = ((bx * xw) + (by * yw) + (bz * zw)) / yw;

        // xyz -> rgb matrix, correctly scaled to white
        m_xyz_to_rgb[0][0] = rx / rw;
        m_xyz_to_rgb[0][1] = ry / rw;
        m_xyz_to_rgb[0][2] = rz / rw;
        m_xyz_to_rgb[1][0] = gx / gw;
        m_xyz_to_rgb[1][1] = gy / gw;
        m_xyz_to_rgb[1][2] = gz / gw;
        m_xyz_to_rgb[2][0] = bx / bw;
        m_xyz_to_rgb[2][1] = by / bw;
        m_xyz_to_rgb[2][2] = bz / bw;

        m_rgb_to_xyz = m_xyz_to_rgb.inverse();
    }
};

RGBf32 xyz_to_rgb(XYZ xyz, const ColorSpaceRGB& cs, bool ignore_transfer=false);
XYZ rgb_to_xyz(RGBf32, const ColorSpaceRGB& cs, bool ignore_transfer=false);

}
