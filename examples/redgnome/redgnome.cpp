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
    turnip::Engine engine(640, 480, "Turnip");
    engine.ResourcesManager().SetResourcesDirectory(std::filesystem::absolute("../../resources"));
    engine.RenderSystem().SetBackgroundColor({52, 67, 94, 255});

    engine.Run();

    return 0;
}
