// Copyright 2025 Valentin Namleev

#pragma once

#include <cstddef>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "./component.hpp"

namespace turnip::ecs {
using EntityID = std::size_t;
class Registry {
public:
  Registry() = default;

  EntityID CreateEntity() {
    EntityID entityID = m_NextEntityID++;
    m_Alive.push_back(entityID);
    return entityID;
  }

  void RemoveEntity(EntityID _EntityID) {
    m_Alive.erase(std::remove(m_Alive.begin(), m_Alive.end(), _EntityID),
                  m_Alive.end());

    for (auto &pair : m_Components) {
      pair.second.erase(_EntityID);
    }
  }

  template <typename T, typename... Args>
  void AddComponent(EntityID _EntityID, T _Component) {
    static_assert(std::is_base_of<IComponent, T>::value,
                  "T must derive from IComponent");
    auto &map = GetComponentMap<T>();
    map[id] = std::make_unique<T>(std::forward<Args>(args)...);
  }

  template <typename T> void RemoveComponent(EntityID _EntityID) {
    getComponentMap<T>().erase(_EntityID);
  }

  template <typename T> T *GetComponent(EntityID _EntityID) {
    auto &map = getComponentMap<T>();
    auto it = map.find(_EntityID);
    return (it != map.end()) ? static_cast<T *>(it->second.get()) : nullptr;
  }

  template <typename T> std::vector<EntityID> With() {
    std::vector<EntityID> result;
    for (auto const &[id, comp] : getComponentMap<T>()) {
      result.push_back(id);
    }
    return result;
  }

private:
  template <typename T>
  std::unordered_map<EntityID, std::unique_ptr<IComponent>> &GetComponentMap() {
    auto type = std::type_index(typeid(T));
    if (m_Components.find(type) == m_Components.end()) {
      m_Components[type] = {};
    }
    return m_Components[type];
  }

  EntityID m_NextEntityID = 1;

  std::vector<EntityID> m_Alive;

  std::unordered_map<std::type_index,
                     std::unordered_map<EntityID, std::unique_ptr<IComponent>>>
      m_Components;
};
} // namespace turnip::ecs
