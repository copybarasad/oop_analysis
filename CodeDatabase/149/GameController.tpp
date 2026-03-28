#include "CommandType.h"
#include "MovementCommand.h"
#include "CastSpellCommand.h"
#include "SimpleCommand.h"
#include "GameController.h"

template<typename InputHandler>
GameController<InputHandler>::GameController(GameEngine* engine) 
    : gameEngine(engine) {}

template<typename InputHandler>
std::unique_ptr<Command> GameController<InputHandler>::getNextCommand() {
    return inputHandler.getNextCommand();
}

template<typename InputHandler>
bool GameController<InputHandler>::processGameCommand(std::unique_ptr<Command>& command) {
    if (!command || !command->isValid()) {
        return false;
    }
    
    switch (command->getType()) {
        case CommandType::MOVE_UP:
        case CommandType::MOVE_DOWN:
        case CommandType::MOVE_LEFT:
        case CommandType::MOVE_RIGHT: {
            auto moveCmd = dynamic_cast<MovementCommand*>(command.get());
            return gameEngine->processPlayerMovement(moveCmd->getDeltaX(), moveCmd->getDeltaY());
        }
        case CommandType::SWITCH_COMBAT_MODE:
            gameEngine->switchPlayerCombatMode();
            return true;
        case CommandType::CAST_SPELL: {
            auto castCmd = dynamic_cast<CastSpellCommand*>(command.get());
            return gameEngine->castSpell(castCmd->getSpellIndex(), castCmd->getTargetPosition());
        }
        case CommandType::BUY_SPELL:
            if (gameEngine->canBuySpell()) {
                gameEngine->buyRandomSpell();
                return true;
            }
            return false;
        default:
            return false;
    }
}