// Copyright 2025 Valentin Namleev

#include <turnip/ecs/component.hpp>

#include <utility>
#include <vector>

using Time = float;
using Value = float;
using ValueIntime = std::pair<Time, Value>;

struct GraphComponent : turnip::ecs::IComponent {
    std::vector<ValueIntime> valuesInTime{};
    float minValue();
    float maxValue();
};