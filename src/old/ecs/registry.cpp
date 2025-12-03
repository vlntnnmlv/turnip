// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/registry.hpp"
#include "feyerverx/ecs/entity.hpp"

namespace feyerverx::ecs {

static EntityID m_nextEntityID = 1;

Entity Registry::createEntity() {
    EntityID entityID = m_nextEntityID++;
    m_alive.push_back(entityID);
    return {entityID, this};
}

void Registry::removeEntity(const EntityID entityID) {
    std::erase(m_alive, entityID);

    for (auto &componentMap : std::ranges::views::values(m_components)) {
        componentMap.erase(entityID);
    }
}

bool Registry::hasComponentByType(const EntityID entityID, const std::type_index t) {
    const auto it = m_components.find(t);
    if (it == m_components.end())
        return false;

    return it->second.contains(entityID);
}
} // namespace feyerverx::ecs
