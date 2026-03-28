#include "InputHandler.h"
#include "CombatSystem.h"
#include "Game.h"
#include "Player.h"
#include "SaveSystem.h"

bool InputHandler::handleInput(char input) {
  Position position = game.getPlayer().getPosition();
  bool moved = false;

  if (targeting.active) {
    switch (input) {
    case 'w':
      if (targeting.cursor.getY() > 0)
        targeting.cursor =
            Position(targeting.cursor.getX(), targeting.cursor.getY() - 1);
      return true;
    case 's':
      targeting.cursor =
          Position(targeting.cursor.getX(), targeting.cursor.getY() + 1);
      return true;
    case 'a':
      if (targeting.cursor.getX() > 0)
        targeting.cursor =
            Position(targeting.cursor.getX() - 1, targeting.cursor.getY());
      return true;
    case 'd':
      targeting.cursor =
          Position(targeting.cursor.getX() + 1, targeting.cursor.getY());
      return true;
    case ' ': {
      Spell *spell = game.getPlayer().getHand().getFirstTargetingSpell();
      if (spell) {
        std::string spellName = spell->name();
        int dx =
            targeting.cursor.getX() - game.getPlayer().getPosition().getX();
        int dy =
            targeting.cursor.getY() - game.getPlayer().getPosition().getY();
        if (dx * dx + dy * dy <= spell->radius() * spell->radius()) {
          if (spell->use(game, targeting.cursor)) {
            targeting.active = false;
            game.getPlayer().getHand().consumeFirstTargetingSpell();
            statusMessage = std::string("Cast ") + spellName + " at (" +
                            std::to_string(targeting.cursor.getX()) + "," +
                            std::to_string(targeting.cursor.getY()) + ")";
            return true;
          }
          statusMessage = "No valid target at selected position";
        }
      }
      targeting.active = false;
      if (statusMessage.empty())
        statusMessage = "Target out of radius";
      return true;
    }
    case 'q':
      targeting.active = false;
      statusMessage = "Cancelled targeting";
      return true;
    default:
      return true;
    }
  }

  switch (input) {
  case 'w':
    position.moveUp();
    moved = true;
    break;
  case 'a':
    position.moveLeft();
    moved = true;
    break;
  case 's':
    position.moveDown();
    moved = true;
    break;
  case 'd':
    position.moveRight();
    moved = true;
    break;
  case 'c':
    game.getPlayer().switchCombatMode();
    return true;
  case 'e':
    if (const Spell *spell =
            game.getPlayer().getHand().getFirstTargetingSpell()) {
      targeting.active = true;
      targeting.radius = spell->radius();
      targeting.cursor = game.getPlayer().getPosition();
      targeting.areaW = spell->areaWidth();
      targeting.areaH = spell->areaHeight();
      statusMessage = std::string("Targeting with ") + spell->name();
    } else {
      if (game.getPlayer().getHand().empty()) {
        statusMessage = "Hand is empty";
      } else {
        statusMessage = "No targetable spells";
      }
    }
    return true;
  case ' ':
    game.getCombatSystem()->playerAttack();
    return true;
  case '1':
    initSaving();
    return true;
  case '2':
    initLoading();
    return true;
  case 'q':
    return false;
  }
  if (moved) {
    if (game.getPlayer().isSlowed()) {
      game.getPlayer().setSlowed(false);
      return true;
    }
    game.movePlayer(position);
  }
  return true;
}

void InputHandler::enableRawMode() {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void InputHandler::disableRawMode() {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_lflag |= (ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char InputHandler::getChar() {
  char c;
  read(STDIN_FILENO, &c, 1);
  return c;
}

void InputHandler::initSaving() {
  disableRawMode();
  std::cout << "Do you really want to save the game (y/n)?\n";
  std::string line;
  std::getline(std::cin, line);
  bool playerWantsToSave = !line.empty() && (line[0] == 'y' || line[0] == 'Y');
  if (playerWantsToSave) {
    std::string saveName;
    std::cout << "Enter the name of save file (without .save): ";
    getline(std::cin, saveName);

    if (!saveName.empty()) {
      try {
        SaveSystem::saveGame(game, saveName);
        statusMessage = "Game saved successfully as: " + saveName + ".save";
      } catch (const std::exception &e) {
        statusMessage = std::string("Save failed: ") + e.what();
      }
    } else {
      statusMessage = "Save cancelled: no filename provided";
    }
  } else {
    statusMessage = "Save cancelled";
  }
  enableRawMode();
}

void InputHandler::initLoading() {
  disableRawMode();
  std::cout << "WARNING: if you load saved game now, unsaved progress will be "
               "discarded\n";
  std::cout << "Do you really want to load new game (y/n)?\n";
  std::string line;
  std::getline(std::cin, line);
  bool playerWantsToLoad = !line.empty() && (line[0] == 'y' || line[0] == 'Y');

  if (playerWantsToLoad) {
    auto saveFiles = SaveSystem::getSaveFiles();
    if (saveFiles.empty()) {
      std::cout << "No save files found.\n";
      enableRawMode();
      statusMessage = "No save files found";
      return;
    }

    std::cout << "\nAvailable save files:\n";
    for (size_t i = 0; i < saveFiles.size(); ++i) {
      std::cout << i + 1 << ". " << saveFiles[i] << "\n";
    }

    std::cout << "\nEnter the number of save file to load (or 0 to cancel): ";
    std::string choiceStr;
    getline(std::cin, choiceStr);

    try {
      int choice = std::stoi(choiceStr);
      if (choice > 0 && choice <= static_cast<int>(saveFiles.size())) {
        std::string selectedFile = saveFiles[choice - 1];
        try {
          SaveSystem::loadGame(game, selectedFile);
          statusMessage = "Game loaded from: " + selectedFile;
        } catch (const std::exception &e) {
          statusMessage = std::string("Load failed: ") + e.what();
        }
      } else if (choice != 0) {
        statusMessage = "Invalid choice";
      } else {
        statusMessage = "Load cancelled";
      }
    } catch (const std::exception &) {
      statusMessage = "Invalid input";
    }
  } else {
    statusMessage = "Load cancelled";
  }
  enableRawMode();
}