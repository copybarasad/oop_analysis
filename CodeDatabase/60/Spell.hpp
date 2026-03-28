#pragma once
#include <Ceil.hpp>
#include <vector>
#include <string>
#include <ScreenSize.hpp>
#include <SpellType.hpp>
#include <LivingEntity.hpp>
#include <memory>

class Player;

class Spell {
public:
    virtual bool apply(std::shared_ptr<Player> player, std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize* screen) = 0;

    virtual std::string getName() const = 0;
    virtual int getCost() const = 0;
    virtual SpellType getType() const = 0; 
};