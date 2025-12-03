// Copyright 2025 Valentin Namleev

#include <functional>
#include <random>
#include <sstream>
#include <utility>

#include "feyerverx/identifiable.hpp"

namespace feyerverx {
UUID::UUID() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    for (auto &b : m_bytes)
        b = static_cast<uint8_t>(gen());
}

bool UUID::operator==(const UUID &other) const noexcept { return m_bytes == other.m_bytes; }

std::array<uint8_t, 16> UUID::bytes() const { return m_bytes; }

IIdentifiable::IIdentifiable(std::string name) : m_name(std::move(name)) {}

const std::string &IIdentifiable::name() const { return m_name; }
UUID IIdentifiable::ID() const { return m_ID; }

} // namespace feyerverx

template <> struct std::hash<feyerverx::UUID> {
    size_t operator()(const feyerverx::UUID &uuid) const noexcept {
        const std::array<uint8_t, 16> &bytes = uuid.bytes();
        const auto *p = reinterpret_cast<const uint64_t *>(bytes.data());
        return std::hash<uint64_t>{}(p[0]) ^ (std::hash<uint64_t>{}(p[1]) << 1);
    }
};