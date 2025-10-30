// Copyright 2025 Valentin Namleev

#pragma once

#include "components/cameraComponent.hpp"

#include <vector>

#include "feyerverx/events/eventManager.hpp"

#include "feyerverx/camera.hpp"
#include "feyerverx/uiBuilder.hpp"

#include "feyerverx/ecs/entity.hpp"
#include "feyerverx/ecs/registry.hpp"
#include "feyerverx/ecs/system.hpp"

namespace feyerverx::ecs {
class Scene : public IIdentifiable {
public:
    explicit Scene(const std::string &id, RectangleOffset viewport,
                   Color backgroundColor = {255, 255, 255, 255}) noexcept;
    ~Scene() = default;

    Scene(Scene &&) noexcept = default;
    Scene &operator=(Scene &&) noexcept = delete;

    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    [[nodiscard]] std::shared_ptr<Registry> registry() const noexcept { return m_registry; }
    [[nodiscard]] Color backgroundColor() const noexcept { return m_backgroundColor; }
    [[nodiscard]] UIBuilder &builder() { return m_builder; };

    [[nodiscard]] Entity addEntity() const;

    void setOrthogonalCamera(Vector2f windowSize, RectangleOffset viewport = {0, 1, 0, 1},
                             float near = 0, float far = 1000, Vector3f position = {},
                             Vector3f lookingAt = {}, Vector3f up = {},
                             Color color = {255, 255, 255, 255}) const;

    void setPerspectiveCamera(float FOV = 90, float near = 0, float far = 1000,
                              Vector3f position = {}, Vector3f lookingAt = {}, Vector3f up = {},
                              Color color = {255, 255, 255, 255}) const;

    template <typename T, typename... Args> void addSystem(Args &&...args) {
        static_assert(std::is_base_of_v<ISystem, T>, "T must derive from ISystem");
        m_systems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    void update(float deltaTime) const;

    bool isActive = false;

private:
    std::shared_ptr<Registry> m_registry{};
    UIBuilder m_builder{m_registry};
    std::vector<std::unique_ptr<ISystem>> m_systems{};
    Color m_backgroundColor;
};
} // namespace feyerverx
