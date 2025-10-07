// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/uiBuilder.hpp"

#include <vector>

#include <SDL3/SDL.h>

#include "feyerverx/ecs/entity.hpp"
#include "feyerverx/ecs/registry.hpp"
#include "feyerverx/ecs/system.hpp"
#include "feyerverx/renderer.hpp"

namespace feyerverx::ecs {
struct ISystem;

class Scene {
public:
    explicit Scene(const std::string &id) noexcept;
    ~Scene() = default;

    Scene(Scene &&) noexcept = default;
    Scene &operator=(Scene &&) noexcept = default;

    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    Entity addEntity();
    void addSystem(std::unique_ptr<ISystem> &&system);

    Registry &registry();
    uiBuilder &builder();

    void update(float deltaTime);
    void enqueueEvent(const SDL_Event &event);

    std::string ID;
    bool isActive = false;

private:
    static size_t nextUID;
    size_t uid;

    Registry m_registry{};
    uiBuilder m_builder{m_registry};
    std::vector<std::unique_ptr<ecs::ISystem>> m_systems{};
};
} // namespace feyerverx
