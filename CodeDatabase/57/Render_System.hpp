#pragma once
#include <SFML/Graphics.hpp>
#include "./Field/Field.hpp"
#include "./Managers/Entity_Manager.hpp"

class Render_System {
private:
    sf::Texture heroIconTexture;
    sf::Font font;

public:
    Render_System();
    void drawField(sf::RenderWindow& window, Field& field);
    void drawEntities(sf::RenderWindow& window, Entity_Manager& entity_manager);

private:
    void drawHealthBar(sf::RenderWindow& window, Entity* entity, const sf::Vector2f& position);
    void drawHeroUI(sf::RenderWindow& window, Hero* hero);
    void drawEntityInfo(sf::RenderWindow& window, Entity* entity, const sf::Vector2f& position);
};