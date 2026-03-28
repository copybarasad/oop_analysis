#include "Render_System.hpp"
#include <iostream>

Render_System::Render_System() {
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Failed to load font in Render_System!" << std::endl;
    }
    
    heroIconTexture.loadFromFile("hero_icon.jpg");
}


void Render_System::drawField(sf::RenderWindow& window, Field& field) {
    auto& cells = field.get_cells();
    auto size = field.get_grid_size();
    
    for (unsigned int x = 0; x < size.x; ++x) {
        for (unsigned int y = 0; y < size.y; ++y) {
            window.draw(cells[x][y]->getSprite());
        }
    }
}

void Render_System::drawHealthBar(sf::RenderWindow& window, Entity* entity, const sf::Vector2f& position) {
    if (!entity || entity->get_helpth() <= 0) return;
    
    const float BAR_WIDTH = 40.0f;
    const float BAR_HEIGHT = 5.0f;
    const float OUTLINE_THICKNESS = 1.0f;
    const float Y_OFFSET = -25.0f;

    int currentHealth = entity->get_helpth();
    int maxHealth = entity->get_max_helpth();
    
    if (maxHealth <= 0) return;
    
    float healthPercentage = static_cast<float>(currentHealth) / maxHealth;
    healthPercentage = std::max(0.0f, std::min(1.0f, healthPercentage));
    
    sf::Vector2f barPosition = position;
    barPosition.y += Y_OFFSET;
    barPosition.x -= BAR_WIDTH / 2.0f;
    
    sf::RectangleShape background(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
    background.setPosition(barPosition);
    background.setFillColor(sf::Color::Black);
    background.setOutlineThickness(OUTLINE_THICKNESS);
    background.setOutlineColor(sf::Color::White);
    
    float healthWidth = BAR_WIDTH * healthPercentage;
    
    sf::RectangleShape healthBar(sf::Vector2f(healthWidth, BAR_HEIGHT));
    healthBar.setPosition(barPosition);
    
    if (healthPercentage > 0.6f) {
        healthBar.setFillColor(sf::Color::Green);
    } else if (healthPercentage > 0.3f) {
        healthBar.setFillColor(sf::Color::Yellow);
    } else {
        healthBar.setFillColor(sf::Color::Red);
    }
    
    window.draw(background);
    if (healthWidth > 0) {
        window.draw(healthBar);
    }
}

void Render_System::drawEntityInfo(sf::RenderWindow& window, Entity* entity, const sf::Vector2f& position) {
    if (!entity) return;
    
    sf::Vector2f textPosition = position;
    textPosition.y -= 40.0f;
    textPosition.x -= 15.0f;
    
    sf::Text levelText;
    levelText.setFont(font);
    levelText.setString("Lv." + std::to_string(entity->getLevel()));
    levelText.setCharacterSize(12);
    levelText.setFillColor(sf::Color::White);
    levelText.setOutlineColor(sf::Color::Black);
    levelText.setOutlineThickness(1);
    levelText.setPosition(textPosition);
    
    window.draw(levelText);
}

void Render_System::drawHeroUI(sf::RenderWindow& window, Hero* hero) {
    if (!hero) return;
    
    const float UI_MARGIN = 20.0f;
    const float UI_WIDTH = 300.0f;
    const float UI_HEIGHT = 100.0f;
    
    sf::Vector2f uiPosition(UI_MARGIN, window.getSize().y - UI_HEIGHT - UI_MARGIN);
    
    sf::RectangleShape background(sf::Vector2f(UI_WIDTH, UI_HEIGHT));
    background.setPosition(uiPosition);
    background.setFillColor(sf::Color(0, 0, 0, 180));
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color::White);
    window.draw(background);
    
    if (heroIconTexture.getSize().x > 0) {
        sf::Sprite heroIcon(heroIconTexture);
        heroIcon.setPosition(uiPosition.x + 10, uiPosition.y + 10);
        float scale = 80.0f / std::max(heroIconTexture.getSize().x, heroIconTexture.getSize().y);
        heroIcon.setScale(scale, scale);
        window.draw(heroIcon);
    } else {
        sf::RectangleShape fallbackIcon(sf::Vector2f(80, 80));
        fallbackIcon.setPosition(uiPosition.x + 10, uiPosition.y + 10);
        fallbackIcon.setFillColor(sf::Color::Blue);
        window.draw(fallbackIcon);
    }
    
    float infoStartX = uiPosition.x + 100;
    
    sf::Text levelText;
    levelText.setFont(font);
    levelText.setString("Level: " + std::to_string(hero->getLevel()));
    levelText.setCharacterSize(18);
    levelText.setFillColor(sf::Color::Yellow);
    levelText.setPosition(infoStartX, uiPosition.y + 10);
    window.draw(levelText);
    
    sf::Text damageText;
    damageText.setFont(font);
    damageText.setString("Damage: " + std::to_string(hero->get_damage_mele()));
    damageText.setCharacterSize(16);
    damageText.setFillColor(sf::Color::White);
    damageText.setPosition(infoStartX, uiPosition.y + 35);
    window.draw(damageText);
    
    if (hero->get_damage_rang() > 0) {
        sf::Text rangeDamageText;
        rangeDamageText.setFont(font);
        rangeDamageText.setString("Range DMG: " + std::to_string(hero->get_damage_rang()));
        rangeDamageText.setCharacterSize(16);
        rangeDamageText.setFillColor(sf::Color::Cyan);
        rangeDamageText.setPosition(infoStartX, uiPosition.y + 55);
        window.draw(rangeDamageText);
    }
    
    const float HP_BAR_WIDTH = 180.0f;
    const float HP_BAR_HEIGHT = 20.0f;
    const float HP_BAR_X = infoStartX;
    const float HP_BAR_Y = uiPosition.y + 75;
    
    int currentHP = hero->get_helpth();
    int maxHP = hero->get_max_helpth();
    float hpPercentage = static_cast<float>(currentHP) / maxHP;
    hpPercentage = std::max(0.0f, std::min(1.0f, hpPercentage));
    
    sf::RectangleShape hpBackground(sf::Vector2f(HP_BAR_WIDTH, HP_BAR_HEIGHT));
    hpBackground.setPosition(HP_BAR_X, HP_BAR_Y);
    hpBackground.setFillColor(sf::Color::Black);
    hpBackground.setOutlineThickness(1);
    hpBackground.setOutlineColor(sf::Color::White);
    window.draw(hpBackground);
    
    sf::RectangleShape hpBar(sf::Vector2f(HP_BAR_WIDTH * hpPercentage, HP_BAR_HEIGHT));
    hpBar.setPosition(HP_BAR_X, HP_BAR_Y);
    
    if (hpPercentage > 0.6f) {
        hpBar.setFillColor(sf::Color::Green);
    } else if (hpPercentage > 0.3f) {
        hpBar.setFillColor(sf::Color::Yellow);
    } else {
        hpBar.setFillColor(sf::Color::Red);
    }
    window.draw(hpBar);
    
    sf::Text hpText;
    hpText.setFont(font);
    hpText.setString(std::to_string(currentHP) + " / " + std::to_string(maxHP));
    hpText.setCharacterSize(14);
    hpText.setFillColor(sf::Color::White);
    hpText.setOutlineColor(sf::Color::Black);
    hpText.setOutlineThickness(1);
    
    sf::FloatRect textBounds = hpText.getLocalBounds();
    hpText.setPosition(HP_BAR_X + (HP_BAR_WIDTH - textBounds.width) / 2, 
                      HP_BAR_Y + (HP_BAR_HEIGHT - textBounds.height) / 2 - 2);
    window.draw(hpText);
}

void Render_System::drawEntities(sf::RenderWindow& window, Entity_Manager& entity_manager) {
    Hero* hero = entity_manager.get_hero();
    if (hero && hero->get_helpth() > 0) {
        window.draw(hero->getSprite());
    }
    
    if (entity_manager.get_statue()) {
        auto* statue = entity_manager.get_statue();
        if (statue->get_helpth() > 0) {
            window.draw(statue->getSprite());
            drawHealthBar(window, statue, statue->getSprite().getPosition());
            drawEntityInfo(window, statue, statue->getSprite().getPosition());
        }
    }
    
    if (entity_manager.get_portal()) {
        auto* portal = entity_manager.get_portal();
        if (portal->get_helpth() > 0) {
            window.draw(portal->getSprite());
            drawHealthBar(window, portal, portal->getSprite().getPosition());
            drawEntityInfo(window, portal, portal->getSprite().getPosition());
        }
    }

    for (auto& enemy : entity_manager.get_enemies()) {
        if (enemy && enemy->get_helpth() > 0) {
            window.draw(enemy->getSprite());
            drawHealthBar(window, enemy, enemy->getSprite().getPosition());
            drawEntityInfo(window, enemy, enemy->getSprite().getPosition());
        }
    }

    for (auto& ally : entity_manager.get_allies()) {
        if (ally && ally->get_helpth() > 0) {
            window.draw(ally->getSprite());
            drawHealthBar(window, ally, ally->getSprite().getPosition());
            drawEntityInfo(window, ally, ally->getSprite().getPosition());
        }
    }
    
    if (hero && hero->get_helpth() > 0) {
        drawHeroUI(window, hero);
    }
}