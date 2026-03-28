#pragma once
#include <Spell.hpp>
#include <memory>

class TowerShot : public Spell {
public:
    TowerShot();

    virtual bool apply(std::shared_ptr<Player> player, std::vector<std::vector<Ceil>>&, std::vector<std::shared_ptr<LivingEntity>>&, const ScreenSize*) override;

    bool apply(std::shared_ptr<Player> player);

    virtual std::string getName() const override;
    virtual int getCost() const override;
    virtual SpellType getType() const override;

private:
    std::string name;
    int cost; 
    SpellType type;
    int damage;
};