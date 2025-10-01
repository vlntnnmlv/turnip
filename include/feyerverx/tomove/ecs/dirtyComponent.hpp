// Copyright 2025 Valentin Namleev

#pragma once

#include "turnip/ecs/component.hpp"

namespace feyerverx::ecs {
struct IDirtyComponent : IComponent {
    bool dirty = false;
};

template <typename T> class DirtyField {
public:
    DirtyField(IDirtyComponent *_Owner, T _Value = T{})
        : m_Owner(_Owner), m_Value(std::move(_Value)) {}

    DirtyField &operator=(const T &_Value) {
        if (m_Value != _Value) {
            m_Value = _Value;
            m_Owner->dirty = true;
        }
        return *this;
    }

    operator const T &() const { return m_Value; }

private:
    IDirtyComponent *m_Owner;
    T m_Value;
};

} // namespace feyerverx::ecs
