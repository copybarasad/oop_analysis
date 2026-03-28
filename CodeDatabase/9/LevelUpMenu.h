#ifndef LEVEL_UP_MENU_H
#define LEVEL_UP_MENU_H

#include <SFML/Graphics.hpp>

enum class LevelUpChoice {
    None,
    IncreaseHp,
    IncreaseMeleeDamage,
    IncreaseDirectSpellDamage
};

class LevelUpMenu {
public:
    LevelUpMenu();

    void Start(int currentLevel, int totalLevels);

    LevelUpChoice HandleEvent(const sf::Event& event);

    void Draw(sf::RenderWindow& window, const sf::Font& font);

private:
    int selected_index_;
    int current_level_;
    int total_levels_;

    sf::Text title_text_;
    sf::Text options_[3];

    void UpdateTexts(const sf::Font& font);
};

#endif