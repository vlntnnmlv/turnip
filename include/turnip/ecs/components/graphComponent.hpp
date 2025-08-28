// Copyright 2025 Valentin Namleev

#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "./turnip/ecs/component.hpp"

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
