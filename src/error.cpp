// Copyright 2025 Valentin Namleev

#include "feyerverx/error.hpp"

namespace feyerverx {
FeyException::FeyException(ErrorType type, const std::string &message)
    : std::runtime_error(message), m_error{type, message} {}

FeyException::FeyException(const Error &error) : std::runtime_error(error.message) {
    m_error = error;
}

FeyException::FeyException(Error &&error) : std::runtime_error(error.message) {
    m_error = std::move(error);
};
}