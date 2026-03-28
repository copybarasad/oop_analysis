#ifndef SPELL_SYSTEM_H
#define SPELL_SYSTEM_H

#include "ISpell.h"
#include "Position.h"
#include <string>
class GameController;
class Hand;
class Player;

class SpellSystem {
public:
    static bool castSpell(ISpell &spell, GameController &controller, const Position &casterPosition, const Position &targetPos);
    static bool validateSpellTarget(const Position &casterPos, const Position &targetPos, int maxRange);
    static bool upgradeSpell(Hand &hand, int spellIndex, Player &player);
    static bool canSpellBeUpgraded(const ISpell* spell);
    static std::string getSpellUpgradeInfo(const ISpell* spell);
};

#endif