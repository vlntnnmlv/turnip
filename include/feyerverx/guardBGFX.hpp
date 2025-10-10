// Copyright 2025 Valentin Namleev

#pragma once

#include "error.hpp"

#include <memory>
#include <variant>

namespace feyerverx {
class GuardBGFX {
public:
    static std::variant<std::unique_ptr<GuardBGFX>, Error> create(void *windowHandle, float width,
                                                                  float height);
    ~GuardBGFX();

private:
    GuardBGFX() = default;
};
} // namespace turnip
