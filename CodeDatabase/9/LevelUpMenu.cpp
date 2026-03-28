#include "LevelUpMenu.h"

LevelUpMenu::LevelUpMenu(): selected_index_(0), current_level_(1), total_levels_(1){
}

void LevelUpMenu::Start(int currentLevel, int totalLevels)
{
    current_level_ = currentLevel;
    total_levels_ = totalLevels;
    selected_index_ = 0;
}

LevelUpChoice LevelUpMenu::HandleEvent(const sf::Event& event)
{
    if (event.type != sf::Event::KeyPressed) {
        return LevelUpChoice::None;
    }

    if (event.key.code == sf::Keyboard::Up) {
        if (selected_index_ > 0) {
            --selected_index_;
        }
    } else if (event.key.code == sf::Keyboard::Down) {
        if (selected_index_ < 2) {
            ++selected_index_;
        }
    } else if (event.key.code == sf::Keyboard::Enter) {
        switch (selected_index_) {
        case 0: return LevelUpChoice::IncreaseHp;
        case 1: return LevelUpChoice::IncreaseMeleeDamage;
        case 2: return LevelUpChoice::IncreaseDirectSpellDamage;
        default: break;
        }
    }

    return LevelUpChoice::None;
}

void LevelUpMenu::UpdateTexts(const sf::Font& font)
{
    title_text_.setFont(font);
    title_text_.setCharacterSize(32);
    title_text_.setFillColor(sf::Color::Yellow);
    title_text_.setString(
        "Level " + std::to_string(current_level_) + "/" + std::to_string(total_levels_) +
        " completed!\nChoose upgrade:"
    );
    title_text_.setPosition(100.f, 60.f);

    const char* labels[3] = {
        "Increase HP",
        "Increase melee damage",
        "Increase direct spell damage"
    };

    for (int i = 0; i < 3; ++i) {
        options_[i].setFont(font);
        options_[i].setCharacterSize(24);
        options_[i].setString(labels[i]);
        options_[i].setFillColor(i == selected_index_ ? sf::Color::Cyan : sf::Color::White);
        options_[i].setPosition(120.f, 150.f + i * 40.f);
    }
}

void LevelUpMenu::Draw(sf::RenderWindow& window, const sf::Font& font)
{
    UpdateTexts(font);

    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(
        static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y)
    ));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    window.draw(title_text_);
    for (int i = 0; i < 3; ++i) {
        window.draw(options_[i]);
    }
}