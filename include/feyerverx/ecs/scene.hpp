// Copyright 2025 Valentin Namleev

#pragma once

#include <vector>

#include <SDL3/SDL.h>

#include "feyerverx/assetManager.hpp"
#include "feyerverx/ecs/entity.hpp"
#include "feyerverx/ecs/registry.hpp"
#include "feyerverx/ecs/system.hpp"
#include "feyerverx/renderer.hpp"

namespace feyerverx::ecs {
struct ISystem;

class Scene {
public:
    std::string id;
    bool isActive;

public:
    Scene() noexcept;
    ~Scene() = default;

    Scene(Scene &&) noexcept = default;
    Scene &operator=(Scene &&) noexcept = default;

    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    ecs::Entity addEntity();
    ecs::Registry &registry();

    void update(float deltaTime);
    void enqueueEvent(const SDL_Event &event);

private:
    static size_t nextuid;
    size_t uid;

    ecs::Registry m_registry{};
    std::vector<std::unique_ptr<ecs::ISystem>> m_systems{};
};
} // namespace feyerverx
