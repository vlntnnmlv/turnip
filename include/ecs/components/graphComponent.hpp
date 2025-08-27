// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"
#include <algorithm>
#include <utility>
#include <vector>

namespace turnip::ecs {
using Time = float;
using Value = float;
using ValueIntime = std::pair<Time, Value>;

struct GraphComponent : IComponent {
    std::vector<ValueIntime> valuesInTime{};

    float minValue();

    float maxValue();
};
} // namespace turnip::ecs
