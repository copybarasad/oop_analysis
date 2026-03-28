#pragma once
#include <Spell.hpp>
#include <memory>    
#include <Player.hpp>

class Player;
class Ceil;
class LivingEntity;
struct ScreenSize;

class EmpowerSpell : public Spell {
public:
    EmpowerSpell();

    virtual bool apply(std::shared_ptr<Player>, std::vector<std::vector<Ceil>>&, std::vector<std::shared_ptr<LivingEntity>>&, const ScreenSize*) override;

    virtual std::string getName() const override;
    virtual int getCost() const override;
    virtual SpellType getType() const override;

private:
    std::string name;
    int cost;
    SpellType type;
};