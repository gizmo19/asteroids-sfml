#include "../include/Core/Core.hpp"
#include "../include/Scenes/MenuScene.hpp"
#include <memory>

int main() {
    Core core;

    auto menuScene = std::make_shared<MenuScene>(&core);
    core.addScene(menuScene);

    core.run();

    return 0;
}
