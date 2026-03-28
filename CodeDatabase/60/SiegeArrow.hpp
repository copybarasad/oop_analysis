#pragma once
#include <Spell.hpp>
#include <SpellType.hpp>
#include <memory>

class SiegeArrow : public Spell {
public:
    SiegeArrow();
    virtual bool apply(std::shared_ptr<Player> player, std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize* screen) override;

    virtual std::string getName() const;
    virtual int getCost() const;
    virtual SpellType getType() const;

private:
    std::string name;
    int cost;
    SpellType type;
};