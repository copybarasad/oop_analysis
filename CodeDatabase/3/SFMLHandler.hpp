#include <SFML/Graphics.hpp>
#include "InputController/Handlers/sfml/KeysConf.hpp"
#include "InputController/commands.hpp"

class SFMLInput {
public:
    SFMLInput(sf::RenderWindow& window);
    Command getCommand();
    bool isAlive();
private:
    sf::RenderWindow& window;
    KeysConf keys;
};
