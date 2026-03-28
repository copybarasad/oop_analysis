#ifndef SETTINGS_MENU_H
#define SETTINGS_MENU_H

#include <SFML/Graphics.hpp>

enum class Difficulty {
    Easy,
    Normal,
    Hard
};

class SettingsMenu {
public:
    SettingsMenu(int& board_width, int& board_height, Difficulty& difficulty);

    bool HandleEvent(const sf::Event& event);

    void Draw(sf::RenderWindow& window, const sf::Font& font) const;

private:
    int& board_width_;
    int& board_height_;
    Difficulty& difficulty_;
    int selected_index_;
};

#endif