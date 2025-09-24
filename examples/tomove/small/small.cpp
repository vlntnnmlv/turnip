// Copyright 2025 Valentin Namleev

#include <turnip/colorUtils.hpp>
#include <turnip/ecs/components/childrenComponent.hpp>
#include <turnip/ecs/components/colorComponent.hpp>
#include <turnip/ecs/components/textComponent.hpp>
#include <turnip/ecs/components/transformComponent.hpp>
#include <turnip/ecs/entity.hpp>
#include <turnip/engine.hpp>

#include <filesystem>
#include <map>
#include <random>
#include <string>

int main() {
    turnip::Engine engine(64, 64, "Turnip");
    engine.ResourcesManager().SetResourcesDirectory(std::filesystem::absolute("../../resources"));

    engine.RenderSystem().SetBackgroundColor({52, 67, 94, 255});
    turnip::UISceneBuilder &sceneBuilder = engine.UISceneBuilder();
    turnip::ResourcesManager &resourcesManager = engine.ResourcesManager();
    turnip::ecs::Entity sceneRoot = sceneBuilder.CreateScene({5, 5, 5, 5});

    sceneBuilder.CreateImage(sceneRoot, resourcesManager.GetSmoothCornerTexture(8),
                             turnip::LRTB{8, 8, 8, 8});

    engine.Run();

    return 0;
}
