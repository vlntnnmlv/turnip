// Copyright 2025 Valentin Namleev

#pragma once

#include "app.hpp"

namespace turnip {
class Turnip : public App {
public:
    Turnip(const char *_title, int _width, int _height);
    virtual ~Turnip() override;

private:
};
} // namespace turnip
