#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "./Field/Field.hpp"
#include "./Entities/Hero.hpp"
#include "./Systems/GameLogger.hpp"

class Entity_Manager;


class InterfaceSpell {
public:
    virtual ~InterfaceSpell() = default;
    virtual bool cast( [[maybe_unused]] Entity& caster, Field& field, InterfaceGameLogger& logger_consol,
         [[maybe_unused]] int x, [[maybe_unused]] int y, Entity_Manager* manager = nullptr) = 0;
    virtual std::string getName() const = 0;
    virtual sf::Sprite get_sprite() const = 0;
    virtual bool requiresTarget() const { return true; }
};
