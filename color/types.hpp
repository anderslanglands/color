#pragma once

#include <cstdint>

#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathMatrix.h>
#include <OpenEXR/half.h>

namespace color {

// Yeah I like Rust's typenames, sue me.
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;

using f16 = half;
using f32 = float;
using f64 = double;

// Import some Imath types directly
using Imath::V2f;
using Imath::V3f;
using Imath::M33f;

using XYZ = Imath::V3f;

}

