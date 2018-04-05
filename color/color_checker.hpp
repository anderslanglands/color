#pragma once

#include "color/spectral_power_distribution.hpp"
#include "color/rgb.hpp"

#include <unordered_map>

namespace color {
namespace ColorChecker {
namespace BabelAverage {
namespace Spectrum {
extern const SPD dark_skin;
extern const SPD light_skin;
extern const SPD blue_sky;
extern const SPD foliage;
extern const SPD blue_flower;
extern const SPD bluish_green;
extern const SPD orange;
extern const SPD purplish_blue;
extern const SPD moderate_red;
extern const SPD purple;
extern const SPD yellow_green;
extern const SPD orange_yellow;
extern const SPD blue;
extern const SPD green;
extern const SPD red;
extern const SPD yellow;
extern const SPD magenta;
extern const SPD cyan;
extern const SPD white_95;
extern const SPD neutral_80;
extern const SPD neutral_65;
extern const SPD neutral_50;
extern const SPD neutral_35;
extern const SPD black_20;
extern const std::unordered_map<std::string, SPD> map;
}
namespace sRGB {
extern const RGBu8 dark_skin;
extern const RGBu8 light_skin;
extern const RGBu8 blue_sky;
extern const RGBu8 foliage;
extern const RGBu8 blue_flower;
extern const RGBu8 bluish_green;
extern const RGBu8 orange;
extern const RGBu8 purplish_blue;
extern const RGBu8 moderate_red;
extern const RGBu8 purple;
extern const RGBu8 yellow_green;
extern const RGBu8 orange_yellow;
extern const RGBu8 blue;
extern const RGBu8 green;
extern const RGBu8 red;
extern const RGBu8 yellow;
extern const RGBu8 magenta;
extern const RGBu8 cyan;
extern const RGBu8 white_95;
extern const RGBu8 neutral_80;
extern const RGBu8 neutral_65;
extern const RGBu8 neutral_50;
extern const RGBu8 neutral_35;
extern const RGBu8 black_20;
extern const std::unordered_map<std::string, RGBu8> map;
}
}
}
}
