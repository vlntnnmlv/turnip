// Copyright 2025 Valentin Namleev

#include "./graphComponent.hpp"

Value GraphComponent::minValue() {
    return std::min_element(
               valuesInTime.begin(), valuesInTime.end(),
               [](const ValueInTime &_A, const ValueInTime &_B) { return _A.second < _B.second; })
        ->second;
}

Value GraphComponent::maxValue() {
    return std::max_element(
               valuesInTime.begin(), valuesInTime.end(),
               [](const ValueInTime &_A, const ValueInTime &_B) { return _A.second < _B.second; })
        ->second;
}
