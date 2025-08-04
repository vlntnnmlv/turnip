// Copyright 2025 Valentin Namleev

#pragma once

#include <cstddef>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "./component.hpp"

namespace turnip::ecs {

using EntityID = std::size_t;
static constexpr EntityID NullEntity = 0;

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
  void AddComponent(EntityID _EntityID, Args &&..._Args) {
    static_assert(std::is_base_of<IComponent, T>::value,
                  "T must derive from IComponent");
    auto &map = GetComponentMap<T>();
    map[_EntityID] = std::make_unique<T>(std::forward<Args>(_Args)...);
  }

  template <typename T> void RemoveComponent(EntityID _EntityID) {
    GetComponentMap<T>().erase(_EntityID);
  }

  template <typename T> T *GetComponent(EntityID _EntityID) {
    auto &map = GetComponentMap<T>();
    auto it = map.find(_EntityID);
    return (it != map.end()) ? static_cast<T *>(it->second.get()) : nullptr;
  }

  template <typename T> std::vector<EntityID> With() {
    std::vector<EntityID> result;
    for (auto const &[id, _] : GetComponentMap<T>()) {
      result.push_back(id);
    }
    return result;
  }

  template <typename T0, typename T1, typename... TRest>
  std::vector<EntityID> With() {
    // 1) Start from the smallest component map, to minimize loops
    auto &map0 = GetComponentMap<T0>();
    auto &map1 = GetComponentMap<T1>();

    // choose the smaller of map0 and map1 as the “driver”
    auto *driver = &map0;
    std::type_index otherType = typeid(T1);
    if (map1.size() < map0.size()) {
      driver = &map1;
      otherType = typeid(T0);
    }

    std::vector<EntityID> result;
    result.reserve(driver->size());

    // 2) For each entity in the driver, check all other components
    for (auto const &[id, _] : *driver) {
      // must have the “other” first
      if (!HasComponentByType(id, otherType))
        continue;

      // then check any remaining TRest...
      if constexpr (sizeof...(TRest) > 0) {
        if (!(HasComponent<TRest>(id) && ...))
          continue;
      }

      // finally, check the driver’s own type if it wasn’t the “other”
      if (otherType == typeid(T0) ? !hasComponent<T1>(id)
                                  : !hasComponent<T0>(id))
        continue;

      result.push_back(id);
    }

    return result;
  }

private:
  template <typename T>
  std::unordered_map<EntityID, std::unique_ptr<IComponent>> &GetComponentMap() {
    auto key = std::type_index(typeid(T));
    auto it = m_Components.find(key);
    if (it == m_Components.end()) {
      auto [newIt, _] = m_Components.emplace(
          key, std::unordered_map<EntityID, std::unique_ptr<IComponent>>{});
      it = newIt;
    }
    return it->second;
  }

  bool HasComponentByType(EntityID _EntyityID, std::type_index _T) {
    auto it = m_Components.find(_T);
    if (it == m_Components.end())
      return false;
    return it->second.find(_EntyityID) != it->second.end();
  }

  template <typename T> bool hasComponent(EntityID _EntyityID) {
    return GetComponent<T>(_EntyityID) != nullptr;
  }

  EntityID m_NextEntityID = 1;

  std::vector<EntityID> m_Alive;

  std::unordered_map<std::type_index,
                     std::unordered_map<EntityID, std::unique_ptr<IComponent>>>
      m_Components;
};
} // namespace turnip::ecs
