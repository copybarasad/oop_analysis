#include "CastSpellCommand.h"
#include "GameState.h"
#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include "InputHandler.h"
#include <iostream>

CastSpellCommand::CastSpellCommand(InputHandler& handler)
    : inputHandler(handler) {
}

bool CastSpellCommand::execute(GameState& gameState) {
    auto& player = gameState.getPlayer();
    if (player.getSpellHand().getSpellCount() == 0) {
        std::cout << "You have no spells in hand!" << std::endl;
        return false;
    }

    int spellChoice = inputHandler.getSpellChoice(player.getSpellHand().getSpellCount());
    if (spellChoice < 1 || spellChoice > player.getSpellHand().getSpellCount()) {
        return false;
    }

    std::pair<int, int> target = inputHandler.getSpellTarget();
    return player.castSpell(spellChoice - 1, target.first, target.second,
        gameState.getField(), gameState.getEnemies());
}

std::string CastSpellCommand::getName() const {
    return "CastSpellCommand";
}