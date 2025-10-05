// Copyright 2025 Valentin Namleev

#pragma once

#include <stdexcept>
#include <string>

namespace feyerverx {
enum class FeyErrorType {
    SDLInitializationError,
    WindowInitializationError,
    GettingNativeWindowHandleError,
    BGFXInitializationError,
    WrongNumberOfArguments
};

class FeyError : public std::runtime_error {
public:
    explicit FeyError(FeyErrorType error, const std::string &message);

private:
    FeyErrorType m_type;
    std::string m_message;
};
} // namespace feyerverx
