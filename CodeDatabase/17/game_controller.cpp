#include "game_controller.h"
#include "game.h"             
#include "console_input_handler.h" 


template<typename InputHandler>
GameController<InputHandler>::GameController(Game* gameInstance)
    : game(gameInstance) {}

template<typename InputHandler>
void GameController<InputHandler>::setGame(Game* g) {
    game = g;
}


template<typename InputHandler>
GameCommand GameController<InputHandler>::getGameCommand() {
    return inputHandler.getGameCommand();
}

template<typename InputHandler>
GameCommand GameController<InputHandler>::getMenuCommand() {
    return inputHandler.getMenuCommand();
}

template<typename InputHandler>
GameCommand GameController<InputHandler>::getSpellCommand() {
    return inputHandler.getSpellCommand();
}

template<typename InputHandler>
GameCommand GameController<InputHandler>::getTargetCommand() {
    return inputHandler.getTargetCommand();
}


template<typename InputHandler>
Position GameController<InputHandler>::processTargetSelection(Position cursor) {
    while (flag) {
        GameCommand cmd = getTargetCommand();
        Position newCursor = cursor;

        switch (cmd.getType()) {
            case GameCommand::Type::MOVE_UP:    newCursor = Position(cursor.getX(), cursor.getY() - 1); break;
            case GameCommand::Type::MOVE_DOWN:  newCursor = Position(cursor.getX(), cursor.getY() + 1); break;
            case GameCommand::Type::MOVE_LEFT:  newCursor = Position(cursor.getX() - 1, cursor.getY()); break;
            case GameCommand::Type::MOVE_RIGHT: newCursor = Position(cursor.getX() + 1, cursor.getY()); break;
            case GameCommand::Type::CONFIRM: return cursor;
            case GameCommand::Type::CANCEL:  return Position(-1, -1);
            default: break;
        }

        if (newCursor.getX() < 0) newCursor = Position(0, newCursor.getY());
        if (newCursor.getY() < 0) newCursor = Position(newCursor.getX(), 0);
        if (newCursor.getX() >= game->getMap().getWidth())  newCursor = Position(game->getMap().getWidth() - 1, newCursor.getY());
        if (newCursor.getY() >= game->getMap().getHeight()) newCursor = Position(newCursor.getX(), game->getMap().getHeight() - 1);

        cursor = newCursor;
    }
    return Position(-1, -1);
}

template<typename InputHandler>
bool GameController<InputHandler>::processGameInput() {
    if (!game) return false;
    GameCommand cmd = inputHandler.getGameCommand();
    switch(cmd.getType()) {
        case GameCommand::Type::MOVE_UP:      game->playerMove(0, -1); return true;
        case GameCommand::Type::MOVE_DOWN:    game->playerMove(0, 1); return true;
        case GameCommand::Type::MOVE_LEFT:    game->playerMove(-1, 0); return true;
        case GameCommand::Type::MOVE_RIGHT:   game->playerMove(1, 0); return true;
        case GameCommand::Type::CAST_SPELL:   return game->initiateSpellCast();
        case GameCommand::Type::TOGGLE_COMBAT:game->playerToggleCombat(); return true;
        case GameCommand::Type::SAVE_GAME:    game->saveGame(); return false;
        case GameCommand::Type::QUIT:         game->setGameRunning(false); return false;
        default: return false;
    }
}

template<typename InputHandler>
void GameController<InputHandler>::processMenu() {
    if (!game) return;
    GameCommand cmd = inputHandler.getMenuCommand();
    switch(cmd.getType()) {
        case GameCommand::Type::MENU_SELECT_1: game->startNewGame(); break;
        case GameCommand::Type::MENU_SELECT_2: game->loadGame(); break;
        case GameCommand::Type::MENU_SELECT_3: game->setGameRunning(false); break;
        default: break;
    }
}

template class GameController<ConsoleInputHandler>;
