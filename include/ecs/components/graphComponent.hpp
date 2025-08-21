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

    float minValue() {
        return std::min_element(valuesInTime.begin(), valuesInTime.end(),
                                [](const ValueIntime &_A, const ValueIntime &_B) {
                                    return _A.second < _B.second;
                                })
            ->second;
    }

    float maxValue() {
        return std::max_element(valuesInTime.begin(), valuesInTime.end(),
                                [](const ValueIntime &_A, const ValueIntime &_B) {
                                    return _A.second < _B.second;
                                })
            ->second;
    }
};
} // namespace turnip::ecs
