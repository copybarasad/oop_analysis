#include "MainMenu.h"

MainMenu::MainMenu()
    : selected_index_(0) {
}

MenuCommand MainMenu::HandleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) {
        return MenuCommand::None;
    }

    if (event.key.code == sf::Keyboard::Up) {
        selected_index_--;
        if (selected_index_ < 0) {
            selected_index_ = 3;
        }
        return MenuCommand::None;
    }

    if (event.key.code == sf::Keyboard::Down) {
        selected_index_++;
        if (selected_index_ > 3) {
        selected_index_ = 0;
        }
        return MenuCommand::None;
    }

    if (event.key.code == sf::Keyboard::Enter ||
        event.key.code == sf::Keyboard::Space) {

        switch (selected_index_) {
        case 0: return MenuCommand::NewGame;
        case 1: return MenuCommand::LoadGame;
        case 2: return MenuCommand::OpenSettings;
        case 3: return MenuCommand::Exit;
        default: return MenuCommand::None;
        }
    }

    if (event.key.code == sf::Keyboard::Escape) {
        return MenuCommand::Exit;
    }

    return MenuCommand::None;
}

void MainMenu::Draw(sf::RenderWindow& window, const sf::Font& font) const {
    sf::Vector2u size = window.getSize();

    sf::RectangleShape bg(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
    bg.setFillColor(sf::Color(20, 20, 20));
    window.draw(bg);

    sf::Text title;
    title.setFont(font);
    title.setString("To Survive in Moscow");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::White);

    auto tb = title.getLocalBounds();
    title.setOrigin(tb.width / 2.f, tb.height / 2.f);
    title.setPosition(size.x / 2.f, size.y * 0.25f);
    window.draw(title);

    const char* labels[] = { "NEW GAME", "LOAD GAME", "SETTINGS", "EXIT" };

    for (int i = 0; i < 4; ++i) {
        sf::Text item;
        item.setFont(font);
        item.setString(labels[i]);
        item.setCharacterSize(32);
        item.setFillColor(i == selected_index_
                          ? sf::Color::Yellow
                          : sf::Color(200, 200, 200));

        auto b = item.getLocalBounds();
        item.setOrigin(b.width / 2.f, b.height / 2.f);
        float y = size.y * 0.45f + static_cast<float>(i) * 50.f;
        item.setPosition(size.x / 2.f, y);
        window.draw(item);
    }
}