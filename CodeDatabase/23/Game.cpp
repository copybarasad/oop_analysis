#include "Game.h"
#include <iostream>
#include <string>
#include <sstream>

Game::Game(int width, int height) : field(width, height), game_running(true) {}

Command Game::parseCommand(const std::string& cmd) {
    static const std::unordered_map<std::string, Command> commandMap = {
        {"w", Command::MOVE_UP},
        {"a", Command::MOVE_LEFT},
        {"s", Command::MOVE_DOWN},
        {"d", Command::MOVE_RIGHT},
        {"h", Command::ATTACK_LEFT},
        {"j", Command::ATTACK_UP},
        {"k", Command::ATTACK_RIGHT},
        {"l", Command::ATTACK_DOWN},
        {"switch", Command::SWITCH_ATTACK},
        {"cast", Command::UNKNOWN},
        {"skip", Command::SKIP},
        {"help", Command::HELP},
        {"quit", Command::QUIT}
    };
    
    auto it = commandMap.find(cmd);
    return (it != commandMap.end()) ? it->second : Command::UNKNOWN;
}

bool Game::handlePlayerMove(int dx, int dy) {
    bool turn_taken = field.movePlayer(dx, dy);
    if (!turn_taken && field.getPlayer() && field.getPlayer()->getIsSlowed()) {
        std::cout << "You are slowed and cannot move this turn!" << std::endl;
        turn_taken = true;
    }
    return turn_taken;
}

void Game::printHelp() {
    std::cout << "\nCommands:" << std::endl;
    std::cout << "w/a/s/d - move up/left/down/right" << std::endl;
    std::cout << "h/j/k/l - attack left/up/right/down" << std::endl;
    std::cout << "switch - switch attack type (costs 1 turn)" << std::endl;
    std::cout << "cast - cast a spell: will ask index and x y" << std::endl;
    std::cout << "skip - skip turn" << std::endl;
    std::cout << "help - show this help" << std::endl;
    std::cout << "quit - exit game" << std::endl;
}

void Game::processTurn() {
    field.removeDeadEnemies();
    field.moveEnemies();
    field.updateBuildings();
    field.updateTowers();
    
    if (field.getPlayer() && field.getPlayer()->getIsSlowed()) {
        field.getPlayer()->setSlowed(false);
    }
}

void Game::run() {
    while (game_running && field.getPlayer() && field.getPlayer()->isAlive()) {
        if (field.getPlayer() && field.getPlayer()->getSlowedNextTurn()) {
            field.getPlayer()->setSlowed(true);
            field.getPlayer()->setSlowedNextTurn(false);
        }
        
        field.display();
        printHelp();
        
        std::cout << "\nEnter command: ";
        
        std::string cmd;
        std::cin >> cmd;
        
        bool turn_taken = false;
        Command commandType = parseCommand(cmd);
        if (cmd == "cast") {
            if (!field.getPlayer()) continue;
            auto* hand = field.getPlayer()->getHand();
            if (!hand || hand->size() == 0) {
                std::cout << "No spells in hand." << std::endl;
            } else {
                const auto& spells = hand->getSpells();
                std::cout << "Spells:" << std::endl;
                for (int i = 0; i < (int)spells.size(); ++i) {
                    std::cout << i << ") " << spells[i]->name() << std::endl;
                }
                std::cout << "Choose index and target x y: ";
                int idx, tx, ty; 
                if (std::cin >> idx >> tx >> ty) {
                    if (hand->castAt(field, *field.getPlayer(), idx, tx, ty)) {
                        turn_taken = true;
                        std::cout << "Spell cast." << std::endl;
                    } else {
                        std::cout << "Spell failed." << std::endl;
                    }
                } else {
                    std::cin.clear();
                    std::string dummy; std::getline(std::cin, dummy);
                    std::cout << "Invalid input." << std::endl;
                }
            }
        } else {
        
        switch (commandType) {
            case Command::MOVE_UP:
                turn_taken = handlePlayerMove(0, -1);
                break;
                
            case Command::MOVE_LEFT:
                turn_taken = handlePlayerMove(-1, 0);
                break;
                
            case Command::MOVE_DOWN:
                turn_taken = handlePlayerMove(0, 1);
                break;
                
            case Command::MOVE_RIGHT:
                turn_taken = handlePlayerMove(1, 0);
                break;
                
            case Command::ATTACK_LEFT:
                turn_taken = field.attackEnemy(-1, 0);
                if (!turn_taken) {
                    std::cout << "No enemy in range to the left!" << std::endl;
                }
                break;
                
            case Command::ATTACK_UP:
                turn_taken = field.attackEnemy(0, -1);
                if (!turn_taken) {
                    std::cout << "No enemy in range above!" << std::endl;
                }
                break;
                
            case Command::ATTACK_RIGHT:
                turn_taken = field.attackEnemy(1, 0);
                if (!turn_taken) {
                    std::cout << "No enemy in range to the right!" << std::endl;
                }
                break;
                
            case Command::ATTACK_DOWN:
                turn_taken = field.attackEnemy(0, 1);
                if (!turn_taken) {
                    std::cout << "No enemy in range below!" << std::endl;
                }
                break;
                
            case Command::SWITCH_ATTACK:
                if (field.getPlayer()) {
                    field.getPlayer()->switchAttackType();
                    turn_taken = true;
                    std::cout << "Attack type switched!" << std::endl;
                }
                break;
                
            case Command::SKIP:
                turn_taken = true;
                break;
                
            case Command::HELP:
                printHelp();
                break;
                
            case Command::QUIT:
                game_running = false;
                break;
                
            case Command::UNKNOWN:
            default:
                std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
                break;
        }
        }
        
        if (turn_taken) {
            processTurn();
        }
    }
    
    if (field.getPlayer() && !field.getPlayer()->isAlive()) {
        std::cout << "\nGame Over! You died!" << std::endl;
    } else {
        std::cout << "\nThanks for playing!" << std::endl;
    }
}
