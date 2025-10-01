// Copyright 2025 Valentin Namleev

#include <turnip/ecs/component.hpp>

#include <utility>
#include <vector>

using Time = float;
using Value = float;
using ValueInTime = std::pair<Time, Value>;

struct GraphSettings {
    float lineThickness = 1.0f;
    float timeSpan = std::numeric_limits<float>::max();
    bool showAxis = false;
    bool invertX = false;
    bool invertY = false;
};

struct GraphComponent : feyerverx::ecs::IComponent {
    std::vector<ValueInTime> valuesInTime{};
    GraphSettings settings;

    GraphComponent(GraphSettings _Settings = GraphSettings{});
    Value minTime();
    Value maxTime();
    Value minValue();
    Value maxValue();
    int firstIndexInTimeSpan();
};