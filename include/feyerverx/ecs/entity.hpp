// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/registry.hpp"

namespace feyerverx::ecs {
class Entity {
public:
    Entity();
    Entity(EntityID entityID, Registry *registry);
    // Entity(const Entity &other) = default;
    // Entity &operator=(const Entity &other) = default;

    [[nodiscard]] EntityID ID() const;

    template <typename T, typename... Args> void addComponent(Args &&...args) {
        return m_Registry->addComponent<T>(m_ID, std::forward<Args>(args)...);
    }

    template <typename T> T *getComponent() const { return m_Registry->getComponent<T>(m_ID); }

    template <typename T> [[nodiscard]] bool hasComponent() const {
        return m_Registry->getComponent<T>(m_ID) != nullptr;
    }

    template <typename T> void remove() const { m_Registry->removeComponent<T>(m_ID); }

    bool operator==(const Entity &other) const { return ID() == other.ID(); }
    bool operator!=(const Entity &other) const { return !(*this == other); }
    bool operator==(const EntityID &otherID) const { return ID() == otherID; }
    bool operator!=(const EntityID &otherID) const { return !(*this == otherID); }

    bool operator<(const Entity &other) const { return m_ID < other.m_ID; }

    operator EntityID() const { return ID(); }

private:
    EntityID m_ID;
    Registry *m_Registry{nullptr};
};
} // namespace feyerverx::ecs