#ifndef SPELL_SYSTEM_H
#define SPELL_SYSTEM_H

#include "Spell.h"
#include "Enhancement.h"
#include "ISpellContext.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Trap.h"
#include "Ally.h"
#include "SpellFactory.h"
#include <vector>
#include <functional>

class SpellSystem {
private:
    std::vector<Enhancement> pendingEnhancements;
    SpellFactory spellFactory;

public:
    SpellSystem();

    bool castSpell(Spell *spell, ISpellContext &context);

    bool castSpellAt(Spell *spell, ISpellContext &context, int targetX, int targetY);

    bool castTowerSpell(int sourceX, int sourceY, int targetX, int targetY, int range, int damage,
                        Field &field, Player &player, std::vector<Enemy> &enemies,
                        std::vector<EnemyBuilding> &buildings,
                        std::function<void(const std::string &)> addMessage);

    void addEnhancement(const Enhancement &enhancement);

    void applyEnhancements(Spell &spell, std::function<void(const std::string &)> addMessage);

    void clearEnhancements();

    Spell *createRandomSpell();

    std::vector<Enhancement> getPendingEnhancements() const;
};

#endif
