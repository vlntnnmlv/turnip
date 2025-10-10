// Copyright 2025 Valentin Namleev

#pragma once

#include <stdexcept>
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

class Exception final : public std::runtime_error {
public:
    explicit Exception(ErrorType type, const std::string &message);

    explicit Exception(const Error &error);
    explicit Exception(Error &&error);
    // TODO: rule of 5

private:
    Error m_error;
};

// TODO: ?? Rework ??
// template <typename T> class ResultOrError {
// public:
//     ResultOrError(const ErrorType type, const std::string &message)
//         : m_data{Error{type, message}}, m_isError{true} {};
//     explicit ResultOrError(const Error &error) : m_data{Error{error}}, m_isError{true} {}
//     explicit ResultOrError(Error &&error) : m_data{Error{std::move(error)}}, m_isError{true} {}
//
//     explicit ResultOrError(T t) : m_data(t), m_isError(false) {}
//     explicit ResultOrError(const T &t) : m_data{t}, m_isError(false) {};
//     explicit ResultOrError(T &&t) : m_data{std::move(t)}, m_isError(false) {};
//
//     operator T() { return m_data; }
//     operator Error() { return m_data; }
//
// private:
//     union Data {
//         T m_result;
//         Error m_error;
//     };
//
//     Data m_data;
//     bool m_isError;
// };
} // namespace feyerverx
