#pragma once
#include <string>
#include <Spell.hpp>
#include <SpellType.hpp>
#include <memory>

class RainOfArrows : public Spell {
public:
    RainOfArrows();

    virtual std::string getName() const;
    virtual SpellType getType() const;
    virtual int getCost() const;
    virtual bool apply(std::shared_ptr<Player> player, std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize* screen);

private:
    std::string name;
    int cost;
    SpellType type;
    int damage;

    void showRadius(int y, int x, int radius, std::vector<std::vector<Ceil>>& ceils, bool switcher);
};