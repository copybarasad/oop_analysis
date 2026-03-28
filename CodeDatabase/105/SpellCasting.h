#ifndef SPELL_CASTING_H
#define SPELL_CASTING_H

#include "Player.h"
#include "Field.h"
#include "Spell.h"
#include "GameVisualize.h"
#include <memory>
#include <iostream>

class SpellCasting {
public:
    static bool castSpell(Player& player, int spellIndex, GameField& field, int targetX, int targetY);
    static std::unique_ptr<Spell> getRandomStarterSpell();
    static bool hasSpells(const Player& player);
    static void displaySpellInfo(const Player& player);
    static bool buySpell(Player& player, std::unique_ptr<Spell> spell, int cost);
};

#endif