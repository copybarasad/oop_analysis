#pragma once
#include <Spell.hpp>

class SummonDecoySpell : public Spell {
public:
    SummonDecoySpell();

    virtual bool apply(std::shared_ptr<Player> player, 
                       std::vector<std::vector<Ceil>>& ceils, 
                       std::vector<std::shared_ptr<LivingEntity>>& entities, 
                       const ScreenSize* screen);

    virtual std::string getName() const override;
    virtual int getCost() const override;
    virtual SpellType getType() const override;

private:
    std::string name;
    int cost;
    SpellType type;
};