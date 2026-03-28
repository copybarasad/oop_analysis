#pragma once
#include <Spell.hpp>
#include <SpellType.hpp>
#include <memory>

class PlaceTrapSpell : public Spell {
public:
    PlaceTrapSpell();
    virtual bool apply(std::shared_ptr<Player> player, std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize* screen) override;

    virtual std::string getName() const override;
    virtual int getCost() const override;
    virtual SpellType getType() const override;

private:
    std::string name;
    int cost;
    SpellType type;
};