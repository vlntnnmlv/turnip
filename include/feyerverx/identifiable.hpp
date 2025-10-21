// Copyright 2025 Valentin Namleev

#pragma once

#include <string>

namespace feyerverx {
class IIdentifiable {
public:
    explicit IIdentifiable(const std::string &id);
    [[nodiscard]] const std::string &ID() const;
    [[nodiscard]] size_t UID() const;

private:
    size_t m_UID;
    std::string m_ID;
};
}