#include "SaveLoadMenu.hpp"
#include <iostream>

SaveLoadMenu::SaveLoadMenu(bool isSaveMenu) 
    : isSaveMenu(isSaveMenu),
     titleText(font),
    backText(font){
}

bool SaveLoadMenu::initialize() {
    if (!font.openFromFile("assets/ArialRegular.ttf")) {
        std::cerr << "Failed to load font for SaveLoadMenu" << std::endl;
        return false;
    }

    saveManager.refreshSaveSlots();

    // Заголовок
    titleText.setFont(font);
    titleText.setString(isSaveMenu ? "SAVE GAME" : "LOAD GAME");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({250.0f, 50.0f});

    // Слоты сохранения (включая автосохранение)
    const auto& slots = saveManager.getSaveSlots();
    slotTexts.clear();

    for (size_t i = 0; i < slots.size(); ++i) {
        sf::Text slotText(font);

        std::string slotInfo;
        if (i == 0) {
            // Автосохранение
            slotInfo = "0 - " + slots[i].displayName;
        } else {
            // Обычные слоты
            slotInfo = std::to_string(i) + " - " + slots[i].displayName;
        }

        if (slots[i].exists) {
            slotInfo += " (Level " + std::to_string(slots[i].level) + ")";
        } else {
            slotInfo += " (Empty)";
        }

        slotText.setString(slotInfo);
        slotText.setCharacterSize(24);
        slotText.setFillColor(slots[i].exists ? sf::Color::Green : sf::Color{128, 128, 128});
        slotText.setPosition({250.0f, 150.0f + i * 50.0f});

        slotTexts.push_back(slotText);
    }

    // Текст возврата
    backText.setFont(font);
    backText.setString("B - Back");
    backText.setCharacterSize(24);
    backText.setFillColor(sf::Color::White);
    backText.setPosition({250.0f, 150.0f + slots.size() * 50.0f});

    return true;
}

int SaveLoadMenu::show(sf::RenderWindow& window) {
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return 0;
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // Обработка клавиш 0-5
                if (keyPressed->scancode == sf::Keyboard::Scancode::Num0) {
                    return 0; // Назад
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Num1) {
                    return 1; // Слот 1
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2) {
                    return 2; // Слот 2
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3) {
                    return 3; // Слот 3
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Num4) {
                    return 4; // Слот 4
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Num5) {
                    return 5; // Слот 5
                }
            }
        }

        window.clear(sf::Color(30, 30, 60));
        window.draw(titleText);
        
        for (const auto& slotText : slotTexts) {
            window.draw(slotText);
        }
        
        window.draw(backText);
        window.display();
    }
    return 0;
}