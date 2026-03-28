#include "EnhancementSpell.hpp"
#include "GameBoard.hpp"
#include "player.hpp"

bool EnhancementSpell::cast(GameBoard& board, Player& player, int /*targetX*/, int /*targetY*/) {
    if (player.getScore() < ENHANCEMENT_SPELL_COST) {
            board.addLogMessage("Not enough score to enhance!");
            return false;
        }
    player.removeScore(ENHANCEMENT_SPELL_COST);
    player.addEnhancementStack();
    board.addLogMessage("You feel empowered! Score spent: " + std::to_string(ENHANCEMENT_SPELL_COST));

    player.addEnhancementStack();
    board.addLogMessage("You feel empowered! Your next spell will be stronger.");
    return true;
}

std::string EnhancementSpell::getName() const { return "Enhance"; }
std::string EnhancementSpell::getDescription() const { return "Empowers your next spell."; }
double EnhancementSpell::getRange() const { return 0.0; } 