#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class GameField;

class WorldObject {
public:
    enum Type {
        TREE,
        ROCK,
        WATER,
        GRASS,
        FLOWER,
        WALL_DECO,
        PILLAR,
        RUG,
        RUNE,
        WALL,
        MUSHROOM,
        STUMP,
        CRYSTAL,
        BONES,
        CRATE
    };

    WorldObject(int x, int y, Type type, sf::Color color = sf::Color::White);
    
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    Type getType() const { return m_type; }
    
    bool isSolid() const;
    std::string getTextureName() const;
    sf::Color getColor() const { return m_color; }
    float getScale() const;
    int getRenderLayer() const; 

    void drawOnField(GameField& field) const;

private:
    int m_x, m_y;
    Type m_type;
    sf::Color m_color;
};