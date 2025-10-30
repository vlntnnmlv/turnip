// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/identifiable.hpp"

#include <any>
#include <ranges>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "feyerverx/ecs/component.hpp"

namespace feyerverx::ecs {
using ComponentTypeSet = std::set<std::type_index>;
using EntityID = std::size_t;
static constexpr EntityID NullEntityID = 0;

class Entity;

class Registry {
public:
    Registry() = default;

    Entity createEntity();

    void removeEntity(EntityID entityID);

    template <typename T, typename... Args> void addComponent(EntityID entityID, Args &&...args) {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from IComponent");
        auto &map = getComponentMap<T>();
        map[entityID] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T> void removeComponent(const EntityID entityID) {
        getComponentMap<T>().erase(entityID);
    }

    template <typename T> T *getComponent(const EntityID entityID) {
        auto &map = getComponentMap<T>();
        auto it = map.find(entityID);
        return it != map.end() ? static_cast<T *>(it->second.get()) : nullptr;
    }

    template <typename T> std::vector<EntityID> with() {
        std::vector<EntityID> result;
        for (auto const &[id, _] : getComponentMap<T>()) {
            result.push_back(id);
        }
        return result;
    }

    std::vector<EntityID> with(const ComponentTypeSet &componentTypeSet) {
        if (componentTypeSet.empty())
            return {};

        const std::type_index *driverType = nullptr;
        size_t minSize = std::numeric_limits<size_t>::max();
        for (const std::type_index &componentType : componentTypeSet) {
            auto it = m_components.find(componentType);
            if (it == m_components.end())
                return {};

            size_t size = it->second.size();
            if (size < minSize) {
                minSize = size;
                driverType = &componentType;
            }
        }

        if (!driverType)
            return {};

        const auto &driverMap = m_components.at(*driverType);

        std::vector<EntityID> result;
        result.reserve(driverMap.size());
        for (const auto &id : std::ranges::views::keys(driverMap)) {
            bool hasAll = true;
            for (const auto &type : componentTypeSet) {
                if (type == *driverType)
                    continue;

                if (!hasComponentByType(id, type)) {
                    hasAll = false;
                    break;
                }
            }

            if (hasAll)
                result.push_back(id);
        }

        return result;
    }

    template <typename T0, typename T1, typename... TRest> std::vector<EntityID> with() {
        auto &map0 = getComponentMap<T0>();
        auto &map1 = getComponentMap<T1>();

        auto *driver = &map0;
        std::type_index otherType = typeid(T1);
        if (map1.size() < map0.size()) {
            driver = &map1;
            otherType = typeid(T0);
        }

        std::vector<EntityID> result;
        result.reserve(driver->size());

        for (auto const &[id, _] : *driver) {
            if (!hasComponentByType(id, otherType))
                continue;

            if constexpr (sizeof...(TRest) > 0) {
                if (!(hasComponent<TRest>(id) && ...))
                    continue;
            }

            if (otherType == typeid(T0) ? !hasComponent<T1>(id) : !hasComponent<T0>(id))
                continue;

            result.push_back(id);
        }

        return result;
    }

private:
    template <typename T>
    std::unordered_map<EntityID, std::unique_ptr<IComponent>> &getComponentMap() {
        auto key = std::type_index(typeid(T));
        auto it = m_components.find(key);
        if (it == m_components.end()) {
            auto [newIt, _] = m_components.emplace(
                key, std::unordered_map<EntityID, std::unique_ptr<IComponent>>{});
            it = newIt;
        }
        return it->second;
    }

    template <typename T> bool hasComponent(const EntityID entityID) {
        return getComponent<T>(entityID) != nullptr;
    }

    bool hasComponentByType(EntityID entityID, std::type_index t);

    std::vector<EntityID> m_alive;

    std::unordered_map<std::type_index, std::unordered_map<EntityID, std::unique_ptr<IComponent>>>
        m_components;
};
} // namespace feyerverx::ecs