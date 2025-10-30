// Copyright 2025 Valentin Namleev

#include <feyerverx/fey.hpp>

#include <feyerverx/ecs/components/spriteComponent.hpp>
#include <feyerverx/ecs/components/transformComponent.hpp>
#include <feyerverx/ecs/entity.hpp>
#include <feyerverx/rectangle.hpp>

#include <feyerverx/ecs/systems/uiSystem.hpp>

using namespace feyerverx;

void add(const ecs::Scene &scene, Texture &tt, float a, float b, float c, float d) {
    auto e = scene.addEntity();
    e.addComponent<ecs::TransformComponent>(Rectangle{a, b, c, d});
    e.addComponent<ecs::SpriteComponent>(tt);
}

struct PlayerComponent : ecs::IComponent {
    float x = 0;
    float y = 0;

    PlayerComponent(float x, float y) : x(x), y(y) {}
};

class PlayerSystem final : public ecs::ISystem {
public:
    explicit PlayerSystem(EventManager &eventManager) : ISystem("PlayerSystem", eventManager) {
        m_eventManager.subscribe<KeyboardEvent>(
            [this](const KeyboardEvent &event) { return onKeyboardEvent(event); });
    }
    ~PlayerSystem() override = default;

    void update(const float deltaTime, const std::shared_ptr<ecs::Registry> &registry) override {

        if (m_playerEntity == ecs::NullEntityID) {
            const auto player = registry->with<PlayerComponent>().front();
            m_playerEntity = ecs::Entity(player, registry.get());
        }

        if (m_cameraEntity == ecs::NullEntityID) {
            const auto camera = registry->with<ecs::CameraComponent>().front();
            m_cameraEntity = ecs::Entity(camera, registry.get());
        }

        const auto playerComponent = m_playerEntity.getComponent<PlayerComponent>();
        playerComponent->x += deltaTime * m_playerSpeed * m_playerDirection.x;
        playerComponent->y += deltaTime * m_playerSpeed * m_playerDirection.y;

        auto *playerTransform = m_playerEntity.getComponent<ecs::TransformComponent>();
        playerTransform->rect.x = playerComponent->x;
        playerTransform->rect.y = playerComponent->y;
    }

    bool onKeyboardEvent(const KeyboardEvent &event) {
        auto rotateCamera = [this](const bool left) -> void {
            if (m_cameraEntity == ecs::NullEntityID)
                return;

            const auto camera = m_cameraEntity.getComponent<ecs::CameraComponent>();
            camera->rotation.y -= left ? 0.1 : -0.1;
        };

        if (event.type == KeyboardEvent::Type::PRESSED) {
            if (event.keycode == SDLK_W) {
                m_playerDirection.y = -1;
            }
            if (event.keycode == SDLK_A) {
                m_playerDirection.x = -1;
            }
            if (event.keycode == SDLK_S) {
                m_playerDirection.y = 1;
            }
            if (event.keycode == SDLK_D) {
                m_playerDirection.x = 1;
            }

            if (event.keycode == SDLK_K) {
                rotateCamera(true);
            }

            if (event.keycode == SDLK_L) {
                rotateCamera(false);
            }
        }

        if (event.type == KeyboardEvent::Type::RELEASED) {
            if (event.keycode == SDLK_W) {
                m_playerDirection.y = 0;
            }
            if (event.keycode == SDLK_A) {
                m_playerDirection.x = 0;
            }
            if (event.keycode == SDLK_S) {
                m_playerDirection.y = 0;
            }
            if (event.keycode == SDLK_D) {
                m_playerDirection.x = 0;
            }
        }

        m_playerDirection.normalize();

        return true;
    }

private:
    ecs::Entity m_playerEntity = ecs::Entity();
    ecs::Entity m_cameraEntity = ecs::Entity();
    Vector2f m_playerDirection{};
    float m_playerSpeed = 200;
};

int main() {
    const Specification specification = {"Fey", {800, 600}};
    auto fey = Fey::create(specification);

    ecs::Scene &mainScene = fey.addScene("main_scene", {0, 1, 0, 1}, {69, 69, 69, 255});
    mainScene.setOrthogonalCamera(specification.windowSize, {0, 1, 0, 1}, 0, 1000, {0, 0, -20},
                                  {0, 0, 0}, {0, -1, 0});

    mainScene.addSystem<PlayerSystem>(fey.eventManager());

    auto devil = fey.assetManager().getTexture("devil");

    auto player = mainScene.addEntity();
    player.addComponent<ecs::SpriteComponent>(devil, RectangleOffset{0, 0, 0, 0});
    player.addComponent<ecs::TransformComponent>(Rectangle{0, 0, 64, 64});
    player.addComponent<PlayerComponent>(300, 400);

    add(mainScene, devil, 0, 0, 400, 400);
    add(mainScene, devil, 100, 55, 40, 40);
    add(mainScene, devil, 400, 33, 100, 100);
    add(mainScene, devil, 200, 300, 200, 300);

    fey.run();

    return 0;
}