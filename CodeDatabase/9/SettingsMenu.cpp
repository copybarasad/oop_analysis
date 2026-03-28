#include "SettingsMenu.h"
#include <sstream>

SettingsMenu::SettingsMenu(int& board_width, int& board_height, Difficulty& difficulty)
    : board_width_(board_width),
      board_height_(board_height),
      difficulty_(difficulty),
      selected_index_(0) {
}

bool SettingsMenu::HandleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) {
        return false;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        return true;
    }

    if (event.key.code == sf::Keyboard::Up) {
        selected_index_--;
        if (selected_index_ < 0) {
            selected_index_ = 2;
        }
        return false;
    }

    if (event.key.code == sf::Keyboard::Down) {
        selected_index_++;
        if (selected_index_ > 2) {
            selected_index_ = 0;
        }
        return false;
    }

    if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {

        int dir = (event.key.code == sf::Keyboard::Right) ? 1 : -1;

        if (selected_index_ == 0) {
            board_width_ += dir;
            if (board_width_ < 10) board_width_ = 10;
            if (board_width_ > 25) board_width_ = 25;
        } else if (selected_index_ == 1) {
            board_height_ += dir;
            if (board_height_ < 10) board_height_ = 10;
            if (board_height_ > 25) board_height_ = 25;
        } else if (selected_index_ == 2) {
            if (dir > 0) {
                if (difficulty_ == Difficulty::Easy) difficulty_ = Difficulty::Normal;
                else if (difficulty_ == Difficulty::Normal) difficulty_ = Difficulty::Hard;
                else difficulty_ = Difficulty::Easy;
            } else {
                if (difficulty_ == Difficulty::Easy) difficulty_ = Difficulty::Hard;
                else if (difficulty_ == Difficulty::Normal) difficulty_ = Difficulty::Easy;
                else difficulty_ = Difficulty::Normal;
            }
        }
        return false;
    }

    if (event.key.code == sf::Keyboard::Enter) {
        return true;
    }

    return false;
}

void SettingsMenu::Draw(sf::RenderWindow& window, const sf::Font& font) const {
    sf::Vector2u size = window.getSize();

    sf::RectangleShape bg(sf::Vector2f(static_cast<float>(size.x),
                                       static_cast<float>(size.y)));
    bg.setFillColor(sf::Color(30, 30, 60));
    window.draw(bg);

    sf::Text title;
    title.setFont(font);
    title.setString("SETTINGS");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::White);
    auto tb = title.getLocalBounds();
    title.setOrigin(tb.width / 2.f, tb.height / 2.f);
    title.setPosition(size.x / 2.f, size.y * 0.2f);
    window.draw(title);

    auto difficultyToString = [this]() -> const char* {
        switch (difficulty_) {
        case Difficulty::Easy:   return "Easy";
        case Difficulty::Normal: return "Normal";
        case Difficulty::Hard:   return "Hard";
        }
        return "Normal";
    };

    std::vector<std::string> lines;

    std::ostringstream oss;


    oss.str("");
    oss.clear();
    oss << "Board width:  " << board_width_;
    lines.push_back(oss.str());


    oss.str("");
    oss.clear();
    oss << "Board height: " << board_height_;
    lines.push_back(oss.str());

    oss.str("");
    oss.clear();
    oss << "Difficulty:   " << difficultyToString();
    lines.push_back(oss.str());

    for (int i = 0; i < static_cast<int>(lines.size()); ++i) {
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(28);
        t.setString(lines[i]);
        t.setFillColor(i == selected_index_
                       ? sf::Color::Yellow
                       : sf::Color(220, 220, 220));

        auto b = t.getLocalBounds();
        t.setOrigin(b.width / 2.f, b.height / 2.f);
        float y = size.y * 0.4f + static_cast<float>(i) * 40.f;
        t.setPosition(size.x / 2.f, y);
        window.draw(t);
    }

    sf::Text hint;
    hint.setFont(font);
    hint.setCharacterSize(18);
    hint.setString("Up/Down: select, Left/Right: change, Enter/Esc: back");
    auto hb = hint.getLocalBounds();
    hint.setOrigin(hb.width / 2.f, hb.height / 2.f);
    hint.setPosition(size.x / 2.f, size.y * 0.85f);
    window.draw(hint);
}