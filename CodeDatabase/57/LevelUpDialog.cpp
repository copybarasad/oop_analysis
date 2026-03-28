#include "LevelUpDialog.hpp"
#include <iostream>

LevelUpDialog::LevelUpDialog(sf::RenderWindow& window, Hero* hero) 
    : window(window), hero(hero), selectedOptionIndex(0) {
    
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Failed to load font in LevelUpDialog!" << std::endl;
    }
    
    loadResources();
    createOptions();
}

void LevelUpDialog::loadResources() {
    heroSprite.setTexture(heroTexture);
    heroSprite.setScale(2.0f, 2.0f);
    
    sf::RenderTexture bgTexture;
    bgTexture.create(window.getSize().x, window.getSize().y);
    sf::RectangleShape bg(sf::Vector2f(window.getSize().x, window.getSize().y));
    bg.setFillColor(sf::Color(0, 0, 0, 180));
    bgTexture.draw(bg);
    bgTexture.display();
    backgroundTexture = bgTexture.getTexture();
    backgroundSprite.setTexture(backgroundTexture);
}

void LevelUpDialog::createOptions() {
    std::vector<std::pair<std::string, std::string>> optionData = {
        {"Increase Level", "Increase hero level by 1\n+ Base stats improvement"},
        {"Upgrade Enhancement", "Upgrade Enhancement spell\n+ Better level scaling"}
    };
    
    float optionWidth = 500.0f;
    float optionHeight = 120.0f;
    float startX = window.getSize().x / 2 - 40 ;
    float startY = window.getSize().y / 2 - (optionData.size() * (optionHeight + 30)) / 2;
    
    for (size_t i = 0; i < optionData.size(); ++i) {
        UpgradeOption option;
        
        option.text.setFont(font);
        option.text.setString(optionData[i].first);
        option.text.setCharacterSize(32);
        option.text.setFillColor(sf::Color::White);
        
        option.description = optionData[i].second;
        
        option.background.setSize(sf::Vector2f(optionWidth, optionHeight));
        option.background.setFillColor(sf::Color(40, 40, 80, 200));
        option.background.setPosition(startX, startY + i * (optionHeight + 30));
        
        option.border.setSize(sf::Vector2f(optionWidth + 8, optionHeight + 8));
        option.border.setFillColor(sf::Color::Transparent);
        option.border.setOutlineThickness(3);
        option.border.setOutlineColor(sf::Color(120, 120, 200));
        option.border.setPosition(startX - 4, startY + i * (optionHeight + 30) - 4);
        
        sf::FloatRect textBounds = option.text.getLocalBounds();
        option.text.setPosition(
            startX + (optionWidth - textBounds.width) / 2,
            startY + i * (optionHeight + 30) + 10
        );
        
        options.push_back(option);
    }
    
    updateOptionAppearance();
}

int LevelUpDialog::show() {
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
                        applyUpgrade();
                        return selectedOptionIndex;

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

void LevelUpDialog::moveUp() {
    if (selectedOptionIndex > 0) {
        selectedOptionIndex--;
        updateOptionAppearance();
    }
}

void LevelUpDialog::moveDown() {
    if (selectedOptionIndex < int(options.size()) - 1) {
        selectedOptionIndex++;
        updateOptionAppearance();
    }
}

void LevelUpDialog::updateOptionAppearance() {
    for (int i = 0; i < int(options.size()); ++i) {
        if (i == selectedOptionIndex) {
            options[i].background.setFillColor(sf::Color(80, 80, 160, 230));
            options[i].border.setOutlineColor(sf::Color::Magenta);
            options[i].border.setOutlineThickness(5);
            options[i].text.setFillColor(sf::Color::Magenta);
            options[i].text.setStyle(sf::Text::Bold);
        } else {
            options[i].background.setFillColor(sf::Color(40, 40, 80, 180));
            options[i].border.setOutlineColor(sf::Color(120, 120, 200));
            options[i].border.setOutlineThickness(3);
            options[i].text.setFillColor(sf::Color::White);
            options[i].text.setStyle(sf::Text::Regular);
        }
    }
}

void LevelUpDialog::applyUpgrade() {
    if (!hero) return;
    
    switch (selectedOptionIndex) {
        case 0:
            hero->setLevel(hero->getLevel() + 1);
            hero->set_helpth(hero->get_max_helpth());
            std::cout << "Hero level increased to " << hero->getLevel() << std::endl;
            break;
            
        case 1:
            hero->UpgradeEnhancementSpellHero();
            std::cout << "Spell level increased to " << hero->get_level_spell() << std::endl;
            break;
    }
}

void LevelUpDialog::draw() {
    window.clear();
    
    window.draw(backgroundSprite);
    
    sf::RectangleShape dialog(sf::Vector2f(1000, 600));
    dialog.setFillColor(sf::Color(30, 30, 60, 220));
    dialog.setOutlineThickness(4);
    dialog.setOutlineColor(sf::Color::Cyan);
    dialog.setPosition(window.getSize().x / 2 - 500, window.getSize().y / 2 - 300);
    window.draw(dialog);
    
    sf::Text title;
    title.setFont(font);
    title.setString("LEVEL UP!");
    title.setCharacterSize(52);
    title.setFillColor(sf::Color::Magenta);
    title.setStyle(sf::Text::Bold);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(3);
    
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(window.getSize().x / 2 - titleBounds.width / 2, 
                     dialog.getPosition().y + 40);
    window.draw(title);
    
    sf::Text heroInfo;
    heroInfo.setFont(font);
    heroInfo.setString("Current Level: " + std::to_string(hero->getLevel()) + 
                      "\nSpell Level: " + std::to_string(hero->getLevelEnhancementSpellHero()));
    heroInfo.setCharacterSize(24);
    heroInfo.setFillColor(sf::Color(200, 200, 255));
    heroInfo.setPosition(dialog.getPosition().x + 60, dialog.getPosition().y + 140);
    window.draw(heroInfo);
    
    heroSprite.setPosition(dialog.getPosition().x + 120, dialog.getPosition().y + 200);
    window.draw(heroSprite);
    
    for (const auto& option : options) {
        window.draw(option.border);
        window.draw(option.background);
        window.draw(option.text);
        
        sf::Text desc;
        desc.setFont(font);
        desc.setString(option.description);
        desc.setCharacterSize(18);
        desc.setFillColor(sf::Color(180, 180, 200));
        
        sf::FloatRect descBounds = desc.getLocalBounds();
        desc.setPosition(
            option.background.getPosition().x + (option.background.getSize().x - descBounds.width) / 2,
            option.background.getPosition().y + option.background.getSize().y - 55
        );
        window.draw(desc);
    }
    
    sf::Text instruction;
    instruction.setFont(font);
    instruction.setString("Use UP/DOWN to choose, ENTER to confirm");
    instruction.setCharacterSize(22);
    instruction.setFillColor(sf::Color(180, 180, 180));
    
    sf::FloatRect instrBounds = instruction.getLocalBounds();
    instruction.setPosition(
        window.getSize().x / 2 - instrBounds.width / 2,
        dialog.getPosition().y + dialog.getSize().y - 50
    );
    window.draw(instruction);
    
    window.display();
}