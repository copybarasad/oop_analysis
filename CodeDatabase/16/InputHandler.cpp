#include "InputHandler.h"
#include "Game.h"
#include <iostream>
#include <algorithm>

void InputHandler::handleInput(Game& game) {
    std::cout << "Enter your move: ";
    char input;
    std::cin >> input;

    if (game.getCurrentState() == GameState::kCasting) {
        handleCastingInput(game, input);
    } else {
        handleNormalInput(game, input);
    }
}

void InputHandler::handleNormalInput(Game& game, char input) {
    bool turn_processed = false;

    switch (input) {
        case 'w': game.movePlayer(0, -1); turn_processed = true; break;
        case 'a': game.movePlayer(-1, 0); turn_processed = true; break;
        case 's': game.movePlayer(0, 1); turn_processed = true; break;
        case 'd': game.movePlayer(1, 0); turn_processed = true; break;
        case 'f': 
            game.getEntityManager().getPlayer()->switchStance();
            std::cout << "Player switched combat stance." << std::endl;
            turn_processed = true; 
            break;
        case 'c':
            game.enterCastingMode();
            break;
        case 'b':
            game.buySpell();
            break;
        case 'k':
            game.saveGame("quicksave.txt");
            break;
        case 'l':
            game.loadGame("quicksave.txt");
            break;
        case 'q':
            game.quit();
            break;
        default:
            std::cout << "Invalid input. Try again." << std::endl;
            break; 
    }
    
    if (turn_processed) {
        game.endPlayerTurn();
    }
}

void InputHandler::handleCastingInput(Game& game, char input) {
    if (input >= '1' && input <= '9') {
        int index = input - '1';
        game.selectSpell(index);
        return;
    }

    Point cursor = game.getCastingCursor();
    switch(input) {
        case 'w': cursor.y = std::max(0, cursor.y - 1); break;
        case 'a': cursor.x = std::max(0, cursor.x - 1); break;
        case 's': cursor.y = std::min(game.getField().getHeight() - 1, cursor.y + 1); break;
        case 'd': cursor.x = std::min(game.getField().getWidth() - 1, cursor.x + 1); break;
        case 'e':
            game.castSelectedSpell();
            break;
        case 'x':
            game.exitCastingMode();
            break;
        default:
            std::cout << "Invalid casting command." << std::endl;
    }
    game.setCastingCursor(cursor);
}