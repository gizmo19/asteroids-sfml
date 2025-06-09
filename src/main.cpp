#include "../include/Core/Core.hpp"
#include "../include/Scenes/GameplayScene.hpp"
#include <memory>

int main() {
    Core core;

    auto gameplayScene = std::make_shared<GameplayScene>(&core.getWindow());
    core.addScene(gameplayScene);

    core.run();

    return 0;
}
