#pragma once
#include <vector>
#include <memory> 
#include <Ceil.hpp>
#include <ScreenSize.hpp>
#include <Spell.hpp>
#include <PlaceTrapSpell.hpp>
#include <SpellType.hpp>

class Player;

class Gauntlet {
public:
    Gauntlet();

    bool learnSpell(std::shared_ptr<Spell> newSpell);

    const std::vector<std::shared_ptr<Spell>>& getKnownSpells() const;

    bool activateMapSkill(int index, std::shared_ptr<Player> player, std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize* screen);

    void randomlyRemoveHalfSpells();

    void clear();
private:

    std::vector<std::shared_ptr<Spell>> spells;
    const int maxSpells; 
};