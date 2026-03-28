#ifndef SPELLCOSTSYSTEM_H
#define SPELLCOSTSYSTEM_H

class Player;
class Spell;

class SpellCostSystem {
public:
    static bool canAffordSpell(const Player& caster, const Spell& spell);
    static void applySpellCost(Player& caster, const Spell& spell);
    static int calculateEffectiveCost(const Player& caster, const Spell& spell);
};

#endif // SPELLCOSTSYSTEM_H