// Copyright 2025 Valentin Namleev

#pragma once

#include <string>

#include "math/vector.hpp"

namespace feyerverx {
struct Specification {
    const std::string name = "Fey";
    Vector2f windowSize = {800, 600};
    bool visual = true;
};

} // namespace feyerverx