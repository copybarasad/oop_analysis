#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "InterfaceSpell.hpp"

class Entity;
class Field;
class InterfaceGameLogger;

class Spell : public virtual InterfaceSpell {
protected:
    sf::Texture texture;
    sf::Sprite sprite;
    inline static int level_spells = 1;
public:
    Spell(const std::string& str_texture) {
        this->texture.loadFromFile(str_texture);
        this->sprite.setTexture(texture);
    }

    virtual ~Spell() = default;

    virtual bool cast(Entity& caster, Field& field, InterfaceGameLogger& logger_consol,
         int x, int y, Entity_Manager* manager = nullptr) = 0;
    virtual std::string getName() const = 0;

    void setLevel(int lvl) { level_spells = lvl; }
    int getLevel() { return level_spells; }

    sf::Sprite get_sprite() const override { return sprite; }
};
