#include "SummonSpell.hpp"
#include "GameBoard.hpp"
#include "player.hpp"
#include "ally.hpp"
#include <cmath>

bool SummonSpell::cast(GameBoard& board, Player& player, int targetX, int targetY) {
    int dx = player.getX() - targetX;
    int dy = player.getY() - targetY;
    double distance = std::sqrt(dx * dx + dy * dy);

    if (distance > getRange()) {
        board.addLogMessage("Cannot summon allies so far away.");
        return false;
    }

    int allies_to_summon = SUMMON_SPELL_ALLY_COUNT + (player.getEnhancementStacks() * SUMMON_SPELL_ALLY_COUNT_BONUS);
    int allies_spawned = 0;

    for (int y_offset = -1; y_offset <= 1; ++y_offset) {
        for (int x_offset = -1; x_offset <= 1; ++x_offset) {
            if (allies_spawned >= allies_to_summon) break;

            int spawnX = targetX + x_offset;
            int spawnY = targetY + y_offset;

            if (board.getEntityAt(spawnX, spawnY) == nullptr) {
                 board.addAlly(new Ally(spawnX, spawnY));
                 allies_spawned++;
            }
        }
        if (allies_spawned >= allies_to_summon) break;
    }

    if (allies_spawned > 0) {
        board.addLogMessage("You have summoned " + std::to_string(allies_spawned) + " ally/allies!");
        player.clearEnhancementStacks();
        return true;
    } else {
        board.addLogMessage("No free space to summon allies.");
        return false;
    }
}

std::string SummonSpell::getName() const { return "Summon Ally"; }
std::string SummonSpell::getDescription() const { return "Summons " + std::to_string(SUMMON_SPELL_ALLY_COUNT) + " ally."; }
double SummonSpell::getRange() const { return SUMMON_SPELL_RANGE; }