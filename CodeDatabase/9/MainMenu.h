#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SFML/Graphics.hpp>

enum class MenuCommand {
    None,
    NewGame,
    LoadGame,
    OpenSettings,
    Exit
};

class MainMenu {
public:
    MainMenu();

    MenuCommand HandleEvent(const sf::Event& event);

    void Draw(sf::RenderWindow& window, const sf::Font& font) const;

private:
    int selected_index_;
};

#endif