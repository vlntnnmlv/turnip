// Copyright 2025 Valentin Namleev

#pragma once

#include <string>

namespace feyerverx {
enum class ErrorType {
    SDLInitializationError,
    WindowInitializationError,
    GettingNativeWindowHandleError,
    BGFXInitializationError,
    WrongNumberOfArguments
};

struct Error {
    ErrorType type;
    std::string message;
};

} // namespace feyerverx
