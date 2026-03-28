#ifndef SPELLVALIDATIONSYSTEM_H
#define SPELLVALIDATIONSYSTEM_H

class Player;
class Spell;
class GameField;

class SpellValidationSystem {
public:
    static bool canCastSpell(const Player& caster, const Spell& spell, const GameField& field);
    static bool hasValidTargets(const Player& caster, const GameField& field, int range);
    static bool hasValidPositions(const Player& caster, const GameField& field, int range);
};

#endif // SPELLVALIDATIONSYSTEM_H