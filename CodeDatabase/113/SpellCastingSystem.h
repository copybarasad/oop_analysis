#ifndef SPELLCASTINGSYSTEM_H
#define SPELLCASTINGSYSTEM_H

#include <string>

class Player;
class GameField;
class Spell;

class SpellCastingSystem {
public:
    static bool castSpellWithManaCheck(Player& caster, GameField& field, Spell& spell);
    static void handleSpellCastResult(bool result, const std::string& spellName);
    static bool validateSpellCastConditions(const Player& caster, const Spell& spell);
};

#endif // SPELLCASTINGSYSTEM_H