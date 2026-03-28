#include "SummonSpell.h"
#include "Game.h"

SummonSpell::SummonSpell(int count, int health, int damage) : count_(count), health_(health), damage_(damage) {}

void SummonSpell::apply(Game& game, int target_x, int target_y) {
    int active_count = count_;
    int boosts = game.getPlayer().consumeBoostCharges();
    if (boosts > 0) {
        active_count += boosts;
    }

    game.spawnAlly(game.getPlayer().getX(), game.getPlayer().getY(), active_count, health_, damage_);

    std::string message = "Summoned " + std::to_string(active_count) + " ally/allies!";
    if (boosts > 0) {
        message += " (Boosted summon!)";
    }
    game.setLastMessage(message);

    game.endPlayerTurn();
}

std::unique_ptr<ISpell> SummonSpell::clone() const {
    return std::make_unique<SummonSpell>(*this);
}

std::string SummonSpell::getName() const { return "Summon Wolf"; }
std::string SummonSpell::getDescription() const {
    return "Summons " + std::to_string(count_) + " ally/allies near the player.";
}
int& SummonSpell::getCount() { return count_; }

int SummonSpell::getTypeId() const {
    return 3;
}
