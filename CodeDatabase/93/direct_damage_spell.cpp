#include "direct_damage_spell.h"
#include "game_controller.h"
#include <cmath>
#include <algorithm>
#include <iostream>

DirectDamageSpell::DirectDamageSpell(int damage, int range)
    : damage_(damage), range_(range) {}

bool DirectDamageSpell::use(GameController& game, int x, int y) {
    const auto& playerPos = game.getPlayer().getPosition();
    const auto& field = game.getField();
    auto& enemies = const_cast<std::vector<Enemy>&>(game.getEnemies());

    int dx = std::abs(x - playerPos.x);
    int dy = std::abs(y - playerPos.y);
    if (dx > range_ || dy > range_) return false;

    auto it = std::find_if(enemies.begin(), enemies.end(), [&](const Enemy& e) {
        return e.isAlive() && e.getPosition().x == x && e.getPosition().y == y;
    });

    if (it != enemies.end()) {
        it->takeDamage(damage_);
        auto& player = const_cast<Player&>(game.getPlayer());
        if (!it->isAlive()) {
            player.addScore(30);
        }
        return true;
    }

    return false;
}

std::string DirectDamageSpell::getName() const { return "Firebolt"; }
std::string DirectDamageSpell::getDescription() const {
    return "Deals " + std::to_string(damage_) + " damage to a single target " 
           + std::to_string(range_) + " cells.";
}

void DirectDamageSpell::save(std::ostream& os) const {
    os << damage_ << " " << range_ << "\n";
}

void DirectDamageSpell::load(std::istream& is) {
    is >> damage_ >> range_;
    std::string leftover;
    std::getline(is, leftover);
}

void DirectDamageSpell::upgrade() {
    damage_ += 10;
    range_ += 1;
}