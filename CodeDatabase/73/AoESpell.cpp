#include "AoESpell.h"
#include "Game.h"
#include <cmath>

AoESpell::AoESpell(int damage, int cast_radius, int area_radius)
        : damage_(damage), cast_radius_(cast_radius), area_radius_(area_radius) {}

void AoESpell::apply(Game& game, int target_x, int target_y) {
    int active_area_radius = area_radius_;
    int boosts = game.getPlayer().consumeBoostCharges();
    if (boosts > 0) {
        // Каждый заряд усиления увеличивает радиус области на 1 (3x3 -> 5x5 -> 7x7)
        active_area_radius += boosts;
    }

    int player_x = game.getPlayer().getX();
    int player_y = game.getPlayer().getY();
    if (std::abs(player_x - target_x) > cast_radius_ || std::abs(player_y - target_y) > cast_radius_) {
        game.setLastMessage("Target is out of range.");
        return;
    }

    std::string message = "Meteor strikes the area!";
    if (boosts > 0) {
        message += " (Boosted area!)";
    }
    game.setLastMessage(message);

    int start_x = target_x - active_area_radius;
    int end_x = target_x + active_area_radius;
    int start_y = target_y - active_area_radius;
    int end_y = target_y + active_area_radius;

    int hits = 0;
    for (int y = start_y; y <= end_y; ++y) {
        for (int x = start_x; x <= end_x; ++x) {
            if (game.damageEntityAt(x, y, damage_)) {
                hits++;
            }
        }
    }

    if(hits > 0) {
        game.setLastMessage(game.getLastMessage() + " Hit " + std::to_string(hits) + " targets.");
    }

    game.endPlayerTurn();
}

std::unique_ptr<ISpell> AoESpell::clone() const {
    return std::make_unique<AoESpell>(*this);
}

std::string AoESpell::getName() const { return "Meteor"; }

std::string AoESpell::getDescription() const {
    int side_length = area_radius_ * 2 + 1;
    return "Deals " + std::to_string(damage_) + " dmg in a " + std::to_string(side_length) + "x" + std::to_string(side_length) + " area. Radius: " + std::to_string(cast_radius_);
}

int AoESpell::getTypeId() const {
    return 1;
}