// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/fey.hpp"

#include "feyerverx/ecs/engines/layoutEngine.hpp"
#include "feyerverx/ecs/system.hpp"

namespace feyerverx::ecs {
class UISystem final : public ISystem {
public:
    explicit UISystem(EventManager &eventManager);
    ~UISystem() override = default;

    void update(float deltaTime, const std::shared_ptr<Registry> &registry) override;

private:
    Vector2f m_size{};
    std::vector<Entity> m_roots;
    LayoutEngine m_layoutEngine{};

    void findRoots(const std::shared_ptr<Registry> &registry);
    void processLayout();

    void measureEntityContent(Entity entity);
    void arrangeEntityContent(Entity entity);
    void placeInWorld(Entity entity);

    bool onWindowResized(const WindowResizedEvent &event);
};
} // namespace feyerverx