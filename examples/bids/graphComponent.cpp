// Copyright 2025 Valentin Namleev

#include "./graphComponent.hpp"
#include <algorithm>

GraphComponent::GraphComponent(GraphSettings _Settings) : settings(_Settings) {}

Time GraphComponent::minTime() {
    return std::ranges::find_if(valuesInTime,
                                [this](ValueInTime &_Pair) -> bool {
                                    return _Pair.first > maxTime() - settings.timeSpan;
                                })
        ->first;
}

Time GraphComponent::maxTime() { return (*(valuesInTime.end() - 1)).first; }

Value GraphComponent::minValue() {
    auto start = std::ranges::find_if(valuesInTime, [this](ValueInTime &_Pair) -> bool {
        return _Pair.first > (*(valuesInTime.end() - 2)).first - settings.timeSpan;
    });

    return std::min_element(
               start, valuesInTime.end(),
               [](const ValueInTime &_A, const ValueInTime &_B) { return _A.second < _B.second; })
        ->second;
}

Value GraphComponent::maxValue() {
    auto start = std::ranges::find_if(valuesInTime, [this](ValueInTime &_Pair) -> bool {
        return _Pair.first > (*(valuesInTime.end() - 2)).first - settings.timeSpan;
    });

    return std::max_element(
               start, valuesInTime.end(),
               [](const ValueInTime &_A, const ValueInTime &_B) { return _A.second < _B.second; })
        ->second;
}

int GraphComponent::firstIndexInTimeSpan() {
    auto it = std::ranges::find_if(valuesInTime, [this](ValueInTime &_Pair) -> bool {
        return _Pair.first > maxTime() - settings.timeSpan;
    });

    return it - valuesInTime.begin();
}
