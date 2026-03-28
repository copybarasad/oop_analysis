#include "trap_spell.h"
#include "game_controller.h"
#include <iostream>

TrapSpell::TrapSpell(int damage) : damage_(damage) {}

bool TrapSpell::use(GameController& game, int x, int y) {
    const auto& field = game.getField();
    const auto& playerPos = game.getPlayer().getPosition();
    auto& enemies = game.getEnemies();

    if (!field.isPositionPassable(x, y)) return false;
    if (playerPos.x == x && playerPos.y == y) return false;
    for (const auto& e : enemies) {
        if (e.isAlive() && e.getPosition().x == x && e.getPosition().y == y)
            return false;
    }

    auto& mutableGame = const_cast<GameController&>(game);
    mutableGame.addTrap(Trap{x, y, damage_});
    return true;
}

std::string TrapSpell::getName() const { return "Spike Trap"; }
std::string TrapSpell::getDescription() const {
    return "Places a trap dealing " + std::to_string(damage_) + " damage when triggered.";
}

void TrapSpell::save(std::ostream& os) const {
    os << damage_ << "\n";
}

void TrapSpell::load(std::istream& is) {
    is >> damage_;
    std::string leftover;
    std::getline(is, leftover);
}

void TrapSpell::upgrade() {
    damage_ += 8;
}