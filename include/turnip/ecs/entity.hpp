// Copyright 2025 Valentin Namleev

#pragma once

#include <cstddef>

#include "./turnip/ecs/registry.hpp"

namespace turnip::ecs {
class Entity {
public:
    Entity();
    Entity(EntityID _ID, Registry *const _Registry);

    EntityID ID() const;

    template <typename T, typename... Args> void AddComponent(Args &&...args) {
        return m_Registry->AddComponent<T>(m_ID, std::forward<Args>(args)...);
    }

    template <typename T> T *GetComponent() const { return m_Registry->GetComponent<T>(m_ID); }

    template <typename T> bool HasComponent() const {
        return m_Registry->GetComponent<T>(m_ID) != nullptr;
    }

    template <typename T> void Remove() { m_Registry->RemoveComponent<T>(m_ID); }

    inline bool operator==(const Entity &_Other) { return ID() == _Other.ID(); }
    inline bool operator!=(const Entity &_Other) { return !(*this == _Other); }
    inline bool operator==(const EntityID &_OtherID) { return ID() == _OtherID; }
    inline bool operator!=(const EntityID &_OtherID) { return !(*this == _OtherID); }

private:
    EntityID m_ID;
    Registry *const m_Registry{nullptr};
};
} // namespace turnip::ecs