// Copyright 2025 Valentin Namleev

#pragma once

#include "error.hpp"

#include <expected>
#include <memory>

namespace feyerverx {
class GuardBGFX {
public:
    static std::expected<std::unique_ptr<GuardBGFX>, Error> create(void *windowHandle, float width,
                                                                   float height);
    ~GuardBGFX();

private:
    GuardBGFX();
};
} // namespace turnip
