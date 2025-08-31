// Copyright 2025 Valentin Namleev

#include "./graphComponent.hpp"

float GraphComponent::minValue() {
    return std::min_element(
               valuesInTime.begin(), valuesInTime.end(),
               [](const ValueIntime &_A, const ValueIntime &_B) { return _A.second < _B.second; })
        ->second;
}

float GraphComponent::maxValue() {
    return std::max_element(
               valuesInTime.begin(), valuesInTime.end(),
               [](const ValueIntime &_A, const ValueIntime &_B) { return _A.second < _B.second; })
        ->second;
}
