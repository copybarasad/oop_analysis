#include "aoe_spell.h"
#include "game_controller.h"
#include <vector>
#include <iostream>

AoESpell::AoESpell(int damage) : damage_(damage) {}

bool AoESpell::use(GameController& game, int x, int y) {
    const auto& field = game.getField();
    auto& player = const_cast<Player&>(game.getPlayer());
    auto& enemies = const_cast<std::vector<Enemy>&>(game.getEnemies());

    if (!field.isPositionValid(x, y)) return false;

    std::vector<std::pair<int, int>> offsets = {{0,0}, {1,0}, {0,1}, {1,1}};

    for (const auto& [dx, dy] : offsets) {
        int tx = x + dx;
        int ty = y + dy;
        if (!field.isPositionValid(tx, ty)) continue;

        for (auto& enemy : enemies) {
            if (enemy.isAlive() && enemy.getPosition().x == tx && enemy.getPosition().y == ty) {
                enemy.takeDamage(damage_);
                if (!enemy.isAlive()) {
                    player.addScore(30);
                }
            }
        }
    }

    return true;
}

std::string AoESpell::getName() const { return "Explosion"; }
std::string AoESpell::getDescription() const {
    return "Deals " + std::to_string(damage_) + " damage in 2x2 area at target.";
}

void AoESpell::save(std::ostream& os) const {
    os << damage_ << "\n";
}

void AoESpell::load(std::istream& is) {
    is >> damage_;
    std::string leftover;
    std::getline(is, leftover);
}

void AoESpell::upgrade() {
    damage_ += 5;
}