// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/components/stackComponent.hpp"

namespace feyerverx::ecs {
StackComponent::StackComponent(StackType _Type, StackContentType _ContentType, float _Spacing)
    : type(_Type), contentType(_ContentType), spacing(_Spacing) {}
} // namespace feyerverx::ecs
