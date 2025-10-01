// Copyright 2025 Valentin Namleev

#include "feyerverx/error.hpp"

namespace feyerverx {
FeyError::FeyError(FeyErrorType error, const std::string &message)
    : std::runtime_error(message), m_type(error) {}
}