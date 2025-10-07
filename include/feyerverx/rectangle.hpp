// Copyright 2025 Valentin Namleev

#pragma once
#include "math/vector.hpp"

namespace feyerverx {
struct Rectangle {
    float x;
    float y;
    float width;
    float height;

    template <arithmetic T> Rectangle move(Vector<T, 2> offset) {
        x += offset.x;
        y += offset.y;

        return *this;
    }
};
} // namespace feyerverx
