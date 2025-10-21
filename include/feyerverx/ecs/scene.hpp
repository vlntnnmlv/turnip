// Copyright 2025 Valentin Namleev

#pragma once

#include <vector>

#include "feyerverx/camera.hpp"
#include "feyerverx/uiBuilder.hpp"

#include "feyerverx/ecs/entity.hpp"
#include "feyerverx/ecs/registry.hpp"
#include "feyerverx/ecs/system.hpp"

namespace feyerverx::ecs {
class Scene : IIdentifiable {
public:
    explicit Scene(const std::string &id, RectangleOffset viewport,
                   Color backgroundColor = {255, 255, 255, 255}) noexcept;
    ~Scene() = default;

    Scene(Scene &&) noexcept = default;
    Scene &operator=(Scene &&) noexcept = delete;

    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    [[nodiscard]] std::shared_ptr<Registry> registry() const noexcept { return m_registry; }
    [[nodiscard]] size_t viewUID() const noexcept { return m_camera->UID(); }
    [[nodiscard]] Color backgroundColor() const noexcept { return m_backgroundColor; }

    Entity addEntity();
    void addSystem(std::unique_ptr<ISystem> &&system);

    std::shared_ptr<Registry> registry();
    UIBuilder &builder();
    std::unique_ptr<ICamera> &camera();

    bool isActive = false;

private:
    Color m_backgroundColor;
    std::unique_ptr<ICamera> m_camera;
    std::shared_ptr<Registry> m_registry{};
    UIBuilder m_builder{m_registry};
    std::vector<std::unique_ptr<ISystem>> m_systems{};
};
} // namespace feyerverx
