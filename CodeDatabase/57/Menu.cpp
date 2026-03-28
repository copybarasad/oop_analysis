#include "Menu.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

Menu::Menu(sf::RenderWindow& window) : window(window), selectedItemIndex(0) {
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }
    backgroundTexture.loadFromFile("./Заставка.png");
    backgroundSprite.setTexture(backgroundTexture);
    
    std::vector<std::string> items = {"New Game", "Load Game", "Exit"};
    
    for (size_t i = 0; i < items.size(); ++i) {
        MenuItem item;
        
        item.text.setFont(font);
        item.text.setString(items[i]);
        item.text.setCharacterSize(60);
        
        sf::FloatRect textBounds = item.text.getLocalBounds();
        item.text.setPosition(window.getSize().x / 2 - textBounds.width / 2, 
                             window.getSize().y / 2 + i * 100);
        
        float paddingX = 40.0f;
        float paddingY = 20.0f;
        item.background.setSize(sf::Vector2f(textBounds.width + paddingX * 2, textBounds.height + paddingY * 2));
        item.background.setPosition(item.text.getPosition().x - paddingX, item.text.getPosition().y - paddingY / 2);
        item.background.setFillColor(sf::Color(30, 30, 60, 180));
        
        item.border.setSize(sf::Vector2f(textBounds.width + paddingX * 2 + 6, textBounds.height + paddingY * 2 + 6));
        item.border.setPosition(item.text.getPosition().x - paddingX - 3, item.text.getPosition().y - paddingY / 2 - 3);
        item.border.setFillColor(sf::Color::Transparent);
        item.border.setOutlineThickness(3);
        item.border.setOutlineColor(sf::Color(100, 100, 200));
        
        menuItems.push_back(item);
    }
    
    updateButtonAppearance();
}

Menu::~Menu() {}

int Menu::show() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return -1;
            }
            
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Up:
                        moveUp();
                        break;
                    case sf::Keyboard::Down:
                        moveDown();
                        break;
                    case sf::Keyboard::Enter:
                        return selectedItemIndex;
                    case sf::Keyboard::Escape:
                        return 2;
                    default:
                        break;
                }
            }
        }
        
        draw();
        sf::sleep(sf::milliseconds(16));
    }
    return -1;
}

void Menu::moveUp() {
    if (selectedItemIndex > 0) {
        selectedItemIndex--;
        updateButtonAppearance();
    }
}

void Menu::moveDown() {
    if (selectedItemIndex < int(menuItems.size()) - 1) {
        selectedItemIndex++;
        updateButtonAppearance();
    }
}

void Menu::updateButtonAppearance() {
    for (int i = 0; i < int(menuItems.size()); ++i) {
        if (i == selectedItemIndex) {
            menuItems[i].background.setFillColor(sf::Color(60, 60, 120, 200));
            menuItems[i].border.setOutlineColor(sf::Color::Magenta);
            menuItems[i].border.setOutlineThickness(4);
            menuItems[i].text.setFillColor(sf::Color::Magenta);
            menuItems[i].text.setStyle(sf::Text::Bold);
            menuItems[i].text.setOutlineColor(sf::Color::Black);
            menuItems[i].text.setOutlineThickness(2);
        } else {
            menuItems[i].background.setFillColor(sf::Color(30, 30, 60, 150));
            menuItems[i].border.setOutlineColor(sf::Color(120, 120, 200));
            menuItems[i].border.setOutlineThickness(2);
            menuItems[i].text.setFillColor(sf::Color::White);
            menuItems[i].text.setStyle(sf::Text::Regular);
            menuItems[i].text.setOutlineColor(sf::Color::Black);
            menuItems[i].text.setOutlineThickness(2);
        }
    }
}

void Menu::draw() {
    window.clear();
    
    window.draw(backgroundSprite);
    
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);
    
    sf::Text title;
    title.setFont(font);
    title.setString("DUNGEON ADVENTURE");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color::Cyan);
    title.setStyle(sf::Text::Bold);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(3);
    
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(window.getSize().x / 2 - titleBounds.width / 2, 100);
    window.draw(title);
    
    sf::Text subtitle;
    subtitle.setFont(font);
    subtitle.setString("Epic Quest");
    subtitle.setCharacterSize(40);
    subtitle.setFillColor(sf::Color(200, 200, 255));
    subtitle.setStyle(sf::Text::Italic);
    
    sf::FloatRect subtitleBounds = subtitle.getLocalBounds();
    subtitle.setPosition(window.getSize().x / 2 - subtitleBounds.width / 2, 200);
    window.draw(subtitle);
    
    for (const auto& item : menuItems) {
        window.draw(item.background);
        window.draw(item.border);
        window.draw(item.text);
    }
    
    sf::Text instruction;
    instruction.setFont(font);
    instruction.setString("Use UP/DOWN arrows to navigate, ENTER to select");
    instruction.setCharacterSize(24);
    instruction.setFillColor(sf::Color(200, 200, 200));
    
    sf::FloatRect instructionBounds = instruction.getLocalBounds();
    instruction.setPosition(window.getSize().x / 2 - instructionBounds.width / 2, 
                           window.getSize().y - 50);
    window.draw(instruction);
    
    window.display();
}