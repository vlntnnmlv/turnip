// Copyright 2025 Valentin Namleev

#pragma once

#include <array>
#include <string>

namespace feyerverx {

struct UUID {
    UUID();

    [[nodiscard]] std::array<uint8_t, 16> bytes() const;
    bool operator==(const UUID &other) const noexcept;

private:
    std::array<uint8_t, 16> m_bytes{};
};

class IIdentifiable {
public:
    explicit IIdentifiable(std::string name = std::string{});
    [[nodiscard]] const std::string &name() const;
    [[nodiscard]] UUID ID() const;

private:
    UUID m_ID{};
    std::string m_name;
};
}