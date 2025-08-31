// Copyright 2025 Valentin Namleev

#pragma once

#include <any>
#include <cstddef>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "./turnip/ecs/component.hpp"

namespace turnip::ecs {

using EntityID = std::size_t;
using ComponentTypeSet = std::set<std::type_index>;

static constexpr EntityID NullEntity = 0;

class Registry {
public:
    Registry() = default;

    EntityID CreateEntity();

    void RemoveEntity(EntityID _EntityID);

    template <typename T, typename... Args> void AddComponent(EntityID _EntityID, Args &&..._Args) {
        static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent");
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

    std::vector<EntityID> With(const ComponentTypeSet &_ComponentTypeSet) {
        if (_ComponentTypeSet.empty())
            return {};

        const std::type_index *driverType = nullptr;
        size_t minSize = std::numeric_limits<size_t>::max();
        for (const std::type_index &componentType : _ComponentTypeSet) {
            auto it = m_Components.find(componentType);
            if (it == m_Components.end())
                return {};

            size_t size = it->second.size();
            if (size < minSize) {
                minSize = size;
                driverType = &componentType;
            }
        }

        if (!driverType)
            return {};

        const auto &driverMap = m_Components.at(*driverType);

        std::vector<EntityID> result;
        result.reserve(driverMap.size());
        for (const auto &[id, _] : driverMap) {
            bool hasAll = true;
            for (const auto &type : _ComponentTypeSet) {
                if (type == *driverType)
                    continue;

                if (!HasComponentByType(id, type)) {
                    hasAll = false;
                    break;
                }
            }

            if (hasAll)
                result.push_back(id);
        }

        return result;
    }

    template <typename T0, typename T1, typename... TRest> std::vector<EntityID> With() {
        auto &map0 = GetComponentMap<T0>();
        auto &map1 = GetComponentMap<T1>();

        auto *driver = &map0;
        std::type_index otherType = typeid(T1);
        if (map1.size() < map0.size()) {
            driver = &map1;
            otherType = typeid(T0);
        }

        std::vector<EntityID> result;
        result.reserve(driver->size());

        for (auto const &[id, _] : *driver) {
            if (!HasComponentByType(id, otherType))
                continue;

            if constexpr (sizeof...(TRest) > 0) {
                if (!(HasComponent<TRest>(id) && ...))
                    continue;
            }

            if (otherType == typeid(T0) ? !HasComponent<T1>(id) : !HasComponent<T0>(id))
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

    template <typename T> bool HasComponent(EntityID _EntyityID) {
        return GetComponent<T>(_EntyityID) != nullptr;
    }

    bool HasComponentByType(EntityID _EntyityID, std::type_index _T);

    EntityID m_NextEntityID = 1;

    std::vector<EntityID> m_Alive;

    std::unordered_map<std::type_index, std::unordered_map<EntityID, std::unique_ptr<IComponent>>>
        m_Components;
};
} // namespace turnip::ecs
