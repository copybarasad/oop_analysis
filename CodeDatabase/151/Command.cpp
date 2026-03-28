#include "Command.h"
#include "GameField.h"
#include "Player.h"
#include "LoadSaveGame.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

MoveCommand::MoveCommand(int dx, int dy) : dx(dx), dy(dy) {}

void MoveCommand::execute(GameField* field, Player* player) {
    if (field && player) {
        field->movePlayer(dx, dy);
    }
}

CastSpellCommand::CastSpellCommand(int index, const Position& target)
    : index(index), target(target) {
}

void CastSpellCommand::execute(GameField* field, Player* player) {
    if (field && player && index >= 0 && index < 5) {
        player->castSpell(index, target, field);
    }
}

void SaveCommand::execute(GameField* field, Player* player) {
    if (field && player) {
        try {
            LoadSaveGame::save(*field, *player, 1, "save.dat");
            std::cout << "Game saved successfully!\n";
        }
        catch (...) {
            std::cout << "Failed to save!\n";
        }
    }
}

void LoadCommand::execute(GameField* field, Player* player) {
    if (field && player) {
        try {
            GameField* loadedField = nullptr;
            Player* loadedPlayer = nullptr;
            int loadedLevel = 1;
            LoadSaveGame::load(loadedField, loadedPlayer, loadedLevel, "save.dat");
            delete field->player; 
            field->player = loadedPlayer; 
            *field = *loadedField; 
            delete loadedField; 

            std::cout << "Game loaded successfully!\n";
        }
        catch (const std::exception& e) {
            std::cout << "Failed to load: " << e.what() << "\n";
        }
    }
}

void BuySpellCommand::execute(GameField*, Player* player) {
    if (player && player->getScore() >= 50) {
        if (player->getHand().addSpell(PlayerHand::getRandomStarterSpell())) {
            player->addScore(-50);
            std::cout << "Bought a random spell!\n";
        }
        else {
            std::cout << "Hand is full!\n";
        }
    }
    else {
        std::cout << "Not enough score! Need 50.\n";
    }
}

void HelpCommand::execute(GameField* field, Player* player) {
    std::cout << "\n=== CURRENT CONTROLS ===\n";
    std::cout << std::left << std::setw(12) << "Key" << " → Action\n";
    std::cout << "──────────────────────────────\n";

    for (const auto& pair : keyMap) {
        char key = pair.first;
        std::string action = pair.second;
        std::string prettyAction = action;

        if (action == "move_up")         prettyAction = "Move Up";
        else if (action == "move_down")  prettyAction = "Move Down";
        else if (action == "move_left")  prettyAction = "Move Left";
        else if (action == "move_right") prettyAction = "Move Right";
        else if (action.substr(0, 5) == "cast_") {
            int idx = std::stoi(action.substr(5)) + 1;
            prettyAction = "Cast Spell " + std::to_string(idx);
        }
        else if (action == "save")   prettyAction = "Save Game";
        else if (action == "load")   prettyAction = "Load Game";
        else if (action == "buy")    prettyAction = "Buy Spell";
        else if (action == "help")   prettyAction = "Show Help";
        else if (action == "quit")   prettyAction = "Quit Game";

        std::cout << "   " << key << "        → " << prettyAction << "\n";
    }

    std::cout << "\nYou can change controls in 'controls.config'\n\n";
}

// === QuitCommand ===
void QuitCommand::execute(GameField*, Player*) {
    std::cout << "=== GAME OVER ===\n";
    std::exit(0); 
}