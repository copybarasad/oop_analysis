#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "InputHandler.h"
#include "Game.h"
#include <iostream>
#include <string>

template<typename InputHandlerType>
class GameController {
private:
  Game& game;
  InputHandlerType inputHandler;
  bool inGameMenu = false;
  
  // игровые команды 
  void executeGameCommand(Command cmd) {
    switch(cmd) {
      case Command::MOVE_UP:
        game.movePlayer(0, -1);
        break;
      case Command::MOVE_DOWN:
        game.movePlayer(0, 1);
        break;
      case Command::MOVE_LEFT:
        game.movePlayer(-1, 0);
        break;
      case Command::MOVE_RIGHT:
        game.movePlayer(1, 0);
        break;
      case Command::ATTACK_MELEE:
        game.playerAttack(AttackType::MELEE);
        break;
      case Command::ATTACK_RANGED:
        game.playerAttack(AttackType::RANGED);
        break;
      case Command::SWITCH_ATTACK:
        game.playerSwitchAttackType();
        break;
      case Command::CAST_SPELL_1:
        game.castSpell(0);
        break;
      case Command::CAST_SPELL_2:
        game.castSpell(1);
        break;
      case Command::CAST_SPELL_3:
        game.castSpell(2);
        break;
      case Command::CAST_SPELL_4:
        game.castSpell(3);
        break;
      case Command::CAST_SPELL_5:
        game.castSpell(4);
        break;
      case Command::SHOW_HELP:
        game.showSpellInfo();
        break;
      case Command::SAVE_MENU:
        enterGameMenu();
        break;
      case Command::QUIT_GAME:
        game.quitGame();
        break;
      case Command::UNKNOWN:
        std::cout << "Unknown command! Press 'h' for help." << std::endl;
        break;
    }
  }

  // команды меню
  void executeMenuCommand(Command cmd) {
    switch(cmd) {
      case Command::CAST_SPELL_1:  
        std::cout << "Saving game..." << std::endl;
        game.saveGame();  
        exitGameMenu();
        break;
      case Command::CAST_SPELL_2:  
        std::cout << "Loading game..." << std::endl;
        game.loadGame();  
        exitGameMenu();
        break;
      case Command::CAST_SPELL_3: 
          exitGameMenu();
          break;
      case Command::QUIT_GAME:    
      case Command::SAVE_MENU:  
          exitGameMenu();
          break;
      default:
          std::cout << "Menu options: 1-Save, 2-Load, 3-Return" << std::endl;
      }
  }
  
  void enterGameMenu() {
    inGameMenu = true;
    showGameMenu();
  }
  
  void exitGameMenu() {
    inGameMenu = false;
    std::cout << "Returning to game..." << std::endl;
  }
  
  void showGameMenu() {
    std::cout << "\n=== IN-GAME MENU ===" << std::endl;
    std::cout << "1. Save Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Return to Game" << std::endl;
    std::cout << "====================" << std::endl;
  }

  bool processMainMenu() {
    bool hasSave = game.getGameSave().saveExists();
    game.setHasSavedGame(hasSave);
    
    while (true) {
      std::cout << "\n=== MAIN MENU ===" << std::endl;
      std::cout << "1. New Game" << std::endl;
      if (hasSave) {
        std::cout << "2. Load Game" << std::endl;
      }
      std::cout << "Choice: ";
      
      std::string choice;
      std::cin >> choice;
      
      if (choice == "1") {
        game.startNewGame();
        return true;
      } else if (choice == "2" && hasSave) {
        try {
          game.loadGame();
          return true;
        } catch (const std::exception& e) {
          std::cout << "Load failed: " << e.what() << std::endl;
          std::cout << "Starting new game instead..." << std::endl;
          game.startNewGame();
          return true;
        }
      } else {
        std::cout << "Invalid choice! Please enter 1" 
                  << (hasSave ? " or 2" : "") 
                  << "." << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
      }
    }
    return false;
  }

public:
  GameController(Game& gameRef, const InputHandlerType& handler) 
      : game(gameRef), inputHandler(handler) {}
  
  void processInput() {
    if (inGameMenu) {
      processMenuInput();
    } else {
      processGameInput();
    }
  }
  
  void processGameInput() {
    std::cout << "\nMove (w/a/s/d), Attack (m/r), Switch attack (t), Spells (1-5), Info (h), Quit (q), Menu(M):";
    
    char input;
    std::cin >> input;
    std::cin.ignore(10000, '\n');
    
    Command cmd = inputHandler.getCommand(input);
    executeGameCommand(cmd);
  }
  
  void processMenuInput() {
    std::cout << "Menu choice (1-3): ";
    
    char input;
    std::cin >> input;
    std::cin.ignore(10000, '\n');
    
    Command cmd = inputHandler.getCommand(input);
    executeMenuCommand(cmd);
  }
  
  bool runMainMenu() {
    return processMainMenu();
  }
  
  bool isInMenu() const {
    return inGameMenu;
  }
};

#endif