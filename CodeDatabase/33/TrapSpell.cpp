#include "TrapSpell.h"
#include "PlayerHand.h"
#include "Game.h"
#include <iostream>
#include <cctype>

TrapSpell::TrapSpell(int damage) : base_damage_(damage), name_("Trap") {}
const std::string& TrapSpell::Name() const { return name_; }

bool TrapSpell::Cast(Game& game, PlayerHand& hand) {
    int bonus = hand.ConsumeBoostForTrapDamage();
    int trap_damage = base_damage_ + bonus;
    if (trap_damage < 1) trap_damage = 1;

    std::cout << "Cast " << name_
              << " (damage " << trap_damage << ").\n"
              << "Place trap next to you. Direction (I/K/J/L, X=cancel): ";

    std::string line;
    if (!std::getline(std::cin, line) || line.empty()) return false;
    char c = static_cast<char>(std::tolower(static_cast<unsigned char>(line[0])));
    if (c == 'x') return false;

    int dx=0, dy=0;
    if (c=='i') dy=-1;
    else if (c=='k') dy= 1;
    else if (c=='j') dx=-1;
    else if (c=='l') dx= 1;
    else return false;

    bool placed = game.PlaceTrapAdj(dx, dy, trap_damage);
    if (!placed) {
        std::cout << "Cannot place trap there.\n";
    }
    return true;
}

std::unique_ptr<Spell> TrapSpell::Clone() const {
    return std::unique_ptr<Spell>(new TrapSpell(base_damage_));
}
