// Copyright 2025 Valentin Namleev

#pragma once

#include <cstdint>

namespace feyerverx {
struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;

    operator uint32_t() const {
        return static_cast<uint32_t>(r) << 24 | static_cast<uint32_t>(g) << 16 |
               static_cast<uint32_t>(b) << 8 | static_cast<uint32_t>(a);
    }
};
} // namespace feyerverx
