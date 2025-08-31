// Copyright 2025 Valentin Namleev

#include <turnip/ecs/component.hpp>

#include <utility>
#include <vector>

using Time = float;
using Value = float;
using ValueInTime = std::pair<Time, Value>;

struct GraphComponent : turnip::ecs::IComponent {
    std::vector<ValueInTime> valuesInTime{};
    Value minValue();
    Value maxValue();
};