#include "DirectDamageSpell.h"
#include "PlayerHand.h"
#include "Game.h"
#include <iostream>
#include <cctype>

DirectDamageSpell::DirectDamageSpell(int damage, int range)
    : base_damage_(damage), base_range_(range), name_("Direct Damage") {}

const std::string& DirectDamageSpell::Name() const { return name_; }

bool DirectDamageSpell::Cast(Game& game, PlayerHand& hand) {
    int bonus_range = hand.ConsumeBoostForDirectRange();
    int range = base_range_ + bonus_range;

    std::cout << "Cast " << name_
              << " (range " << range
              << ", damage " << base_damage_ << ")\n"
              << "Direction (I=up, K=down, J=left, L=right, X=cancel): ";

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

    bool hit = game.SpellDirectDamageRay(dx, dy, range, base_damage_);
    if (!hit) {
        std::cout << "No valid target in that direction.\n";
    }
    return true;
}

std::unique_ptr<Spell> DirectDamageSpell::Clone() const {
    return std::unique_ptr<Spell>(new DirectDamageSpell(base_damage_, base_range_));
}
