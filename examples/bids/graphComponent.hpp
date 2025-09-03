// Copyright 2025 Valentin Namleev

#include <turnip/ecs/component.hpp>

#include <utility>
#include <vector>

using Time = float;
using Value = float;
using ValueInTime = std::pair<Time, Value>;

struct GraphSettings {
    float lineThickness = 1.0f;
};

struct GraphComponent : turnip::ecs::IComponent {
    std::vector<ValueInTime> valuesInTime{};
    GraphSettings settings{};
    Value minValue();
    Value maxValue();
};