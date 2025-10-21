// Copyright 2025 Valentin Namleev

#include "feyerverx/identifiable.hpp"

namespace feyerverx {
IIdentifiable::IIdentifiable(const std::string &id) : m_ID(id) {
    static size_t nextUID = 0;
    m_UID = nextUID++;
}

const std::string &IIdentifiable::ID() const { return m_ID; }
size_t IIdentifiable::UID() const { return m_UID; }

}