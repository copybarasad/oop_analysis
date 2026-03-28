#include "AreaDamageSpell.h"
#include "PlayerHand.h"
#include "Game.h"
#include <iostream>
#include <sstream>

AreaDamageSpell::AreaDamageSpell(int damage, int base_size)
    : damage_(damage), base_size_(base_size), name_("Area Damage") {}

const std::string& AreaDamageSpell::Name() const { return name_; }

bool AreaDamageSpell::Cast(Game& game, PlayerHand& hand) {
    int extra = hand.ConsumeBoostForAreaSize();
    int size = base_size_ + extra;
    if (size < 1) size = 1;

    std::cout << "Cast " << name_
              << " (square " << size << "x" << size
              << ", damage " << damage_ << ")\n"
              << "Enter top-left cell as 'x y' or 'cancel': ";

    std::string line;
    if (!std::getline(std::cin, line) || line.empty()) return false;
    if (line == "cancel" || line == "CANCEL") return false;

    std::istringstream iss(line);
    int tx=0, ty=0;
    if (!(iss >> tx >> ty)) {
        std::cout << "Bad coordinates.\n";
        return false;
    }

    game.AreaDamageAt(Coord(tx, ty), size, damage_);
    return true;
}

std::unique_ptr<Spell> AreaDamageSpell::Clone() const {
    return std::unique_ptr<Spell>(new AreaDamageSpell(damage_, base_size_));
}
