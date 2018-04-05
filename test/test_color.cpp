#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>

#include <color/color_checker.hpp>
#include <color/color_space_rgb.hpp>
#include <color/rgb.hpp>
#include <color/spd_conversion.hpp>

TEST_CASE("BabelAverage spectral to u8 sRGB matches", "[color]") {

    auto xyz = spd_to_xyz(color::Illuminant::D65, color::ColorSpaceRGB::ITUR_sRGB.cmf);
    auto rgb = xyz_to_rgb(xyz, color::ColorSpaceRGB::ITUR_sRGB);
    fmt::print("{:<20}: {}\n", "Illuminant", rgb);

    
    for (const auto& p : color::ColorChecker::BabelAverage::Spectrum::map) {
        auto rgb = color::rgb_cast<color::u8>(
            spd_to_rgb(p.second, color::ColorSpaceRGB::ITUR_sRGB));
        REQUIRE(rgb ==
                color::ColorChecker::BabelAverage::sRGB::map.at(p.first));
    }
}
