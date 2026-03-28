#include "StartMenu.hpp"
#include <iostream>

StartMenu::StartMenu() 
    : saveExists(false),
    titleText(font),
    newGameText(font),
    loadGameText(font),
    exitText(font),
    noSaveText(font){
}

bool StartMenu::initialize() {
    // Загрузка шрифта
    if (!font.openFromFile("assets/ArialRegular.ttf")) {
        std::cerr << "Failed to load font for StartMenu" << std::endl;
        return false;
    }

    // Проверка существования сохранения
    saveExists = fs::exists("autosave.dat") || fs::exists("savegame_1.dat");

    // Настройка текстов
    titleText.setFont(font);
    titleText.setString("=== OOP GAME ===");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({250.0f, 100.0f});

    newGameText.setFont(font);
    newGameText.setString("1 - New Game");
    newGameText.setCharacterSize(30);
    newGameText.setFillColor(sf::Color::Green);
    newGameText.setPosition({300.0f, 200.0f});

    loadGameText.setFont(font);
    loadGameText.setString("2 - Load Game");
    loadGameText.setCharacterSize(30);
    loadGameText.setFillColor(saveExists ? sf::Color::Cyan : sf::Color{128, 128, 128});
    loadGameText.setPosition({300.0f, 250.0f});

    exitText.setFont(font);
    exitText.setString("3 - Exit");
    exitText.setCharacterSize(30);
    exitText.setFillColor(sf::Color::Red);
    exitText.setPosition({300.0f, 300.0f});

    noSaveText.setFont(font);
    noSaveText.setString("No save game found");
    noSaveText.setCharacterSize(20);
    noSaveText.setFillColor(sf::Color::Red);
    noSaveText.setPosition({300.0f, 280.0f});

    return true;
}

int StartMenu::show(sf::RenderWindow& window) {
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return 3;
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Num1) {
                    return 1;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2 && saveExists) {
                    return 2;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3) {
                    return 3;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    return 3;
                }
            }
        }

        window.clear(sf::Color(30, 30, 60));
        window.draw(titleText);
        window.draw(newGameText);
        window.draw(loadGameText);
        window.draw(exitText);

        if (!saveExists) {
            window.draw(noSaveText);
        }

        window.display();
    }
    return 3;
}