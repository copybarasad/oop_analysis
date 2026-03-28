#include "DirectDamageSpell.h"
#include "Game.h"
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int damage, int radius) : damage_(damage), radius_(radius) {}

void DirectDamageSpell::apply(Game& game, int target_x, int target_y) {
    int active_radius = radius_;
    int boosts = game.getPlayer().consumeBoostCharges();
    if (boosts > 0) {
        active_radius += boosts * 5; // Каждый заряд улучшения увеличивает радиус на 1
    }

    int player_x = game.getPlayer().getX();
    int player_y = game.getPlayer().getY();
    if (std::abs(player_x - target_x) > active_radius || std::abs(player_y - target_y) > active_radius) {
        game.setLastMessage("Target is out of range.");
        return;
    }


    if (game.damageEntityAt(target_x, target_y, damage_)) {
        std::string message = "Fireball hits for " + std::to_string(damage_) + " damage!";
        if (boosts > 0) {
            message += " (Boosted radius!)";
        }
        game.setLastMessage(message);
        game.endPlayerTurn();
    } else {
        game.setLastMessage("There is no valid target at that location.");
    }
}

std::unique_ptr<ISpell> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(*this);
}

std::string DirectDamageSpell::getName() const { return "Fireball"; }
std::string DirectDamageSpell::getDescription() const {
    return "Deals " + std::to_string(damage_) + " dmg to one target. Radius: " + std::to_string(radius_);
}
int& DirectDamageSpell::getRadius() { return radius_; }

int DirectDamageSpell::getTypeId() const {
    return 0;
}