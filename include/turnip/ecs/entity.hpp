// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/ecs/registry.hpp"

namespace turnip::ecs {
class Entity {
public:
    Entity(Registry &_Registry);

    EntityID ID() const;

    template <typename T, typename... Args> T *Add(Args &&...args) {
        return m_Registry.AddComponent<T>(m_ID, std::forward<Args>(args)...);
    }

    template <typename T> T *Get() const { return m_Registry.GetComponent<T>(m_ID); }

    template <typename T> bool Has() const { return m_Registry.GetComponent<T>(m_ID) != nullptr; }

    template <typename T> void Remove() { m_Registry.RemoveComponent<T>(m_ID); }

private:
    EntityID m_ID;
    Registry &m_Registry;
};
} // namespace turnip::ecs