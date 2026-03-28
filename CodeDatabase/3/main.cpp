#include <SFML/Graphics.hpp>
#include <memory>
#include "GameClass/GameClass.hpp"
#include "InputController/CommandManager.hpp"
#include "ViewController/ViewManager.hpp"
#include "ViewController/Renderers/ConsoleRenderer.hpp"
#include "ViewController/Renderers/SFMLRenderer.hpp"
#include "InputController/Handlers/sfml/SFMLHandler.hpp"
#include "InputController/Handlers/console/ConsoleHandler.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(CFG::WINDOW_WIDTH, CFG::WINDOW_HEIGHT), CFG::WINDOW_NAME);

    GameClass game;

    CommandManager<SFMLInput> input(game, window);
    ViewManager<SFMLRenderer> output(game, window);
    
    // CommandManager<ConsoleInput> input(game);
    // ViewManager<ConsoleRenderer> output(game);

    while (input.isAlive() && output.isAlive()) {
        output.render();
        input.handle();
    }

    return 0;
}
