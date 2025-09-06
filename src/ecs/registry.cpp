// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/registry.hpp"
#include "./turnip/ecs/entity.hpp"

namespace turnip::ecs {

Entity Registry::CreateEntity() {
    EntityID entityID = m_NextEntityID++;
    m_Alive.push_back(entityID);
    return Entity(entityID, this);
}

void Registry::RemoveEntity(EntityID _EntityID) {
    m_Alive.erase(std::remove(m_Alive.begin(), m_Alive.end(), _EntityID), m_Alive.end());

    for (auto &pair : m_Components) {
        pair.second.erase(_EntityID);
    }
}

bool Registry::HasComponentByType(EntityID _EntyityID, std::type_index _T) {
    auto it = m_Components.find(_T);
    if (it == m_Components.end())
        return false;
    return it->second.find(_EntyityID) != it->second.end();
}
} // namespace turnip::ecs
