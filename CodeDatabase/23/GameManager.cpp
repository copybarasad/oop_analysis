#include "GameManager.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "EventSystem.h"
#include "GameEvent.h"
#include "GameExceptions.h"
#include "InputReader.h"
#include "Spell.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

Spell *makeDirectSpell(int damage = DIRECT_SPELL_DAMAGE,
                       int range = DIRECT_SPELL_RANGE);
Spell *makeAreaSpell();
Spell *makeTrapSpell();

static void waitForEnter() {
  std::cout << "\nPress Enter to continue...";
  std::cout.flush();
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cin.get();
}

GameManager::GameManager()
    : currentLevel(1), gameRunning(false), hasSavedPlayerState(false),
      savedPlayerHp(PLAYER_DEFAULT_HP),
      savedPlayerBaseDamage(PLAYER_DEFAULT_BASE_DMG),
      savedPlayerHandCapacity(PLAYER_HAND_CAPACITY), savedPlayerScore(0),
      savedAttackType(AttackType::MELEE) {
  
  inputReader = std::make_unique<ConsoleInputReader>(this, "keybindings.json");
}

GameManager::~GameManager() {}

int GameManager::getLevelFieldSize(int level) const {
  int size = BASE_FIELD_SIZE + (level - 1) * LEVEL_SIZE_INCREMENT;
  return std::min(size, MAX_FIELD_SIZE);
}

int GameManager::getLevelEnemyCount(int level) const {
  return static_cast<int>(ENEMY_MIN_COUNT *
                          std::pow(LEVEL_ENEMY_COUNT_MULTIPLIER, level - 1));
}

int GameManager::getLevelEnemyHpBonus(int level) const {
  return (level - 1) * LEVEL_ENEMY_HP_BONUS;
}

int GameManager::getLevelEnemyDmgBonus(int level) const {
  return (level - 1) * LEVEL_ENEMY_DMG_BONUS;
}

void GameManager::startGame() {
  gameRunning = true;
  currentLevel = 1;
  hasSavedPlayerState = false;
  savedPlayerHp = PLAYER_DEFAULT_HP;
  savedPlayerBaseDamage = PLAYER_DEFAULT_BASE_DMG;
  savedPlayerHandCapacity = PLAYER_HAND_CAPACITY;
  savedPlayerScore = 0;
  savedAttackType = AttackType::MELEE;
  savedSpells.clear();

  if (askToLoad()) {
    try {
      loadGame();
      std::cout << "Game loaded successfully!" << std::endl;
      runLevel(currentLevel);
      return;
    } catch (const FileNotFoundException &e) {
      std::cout << "\n!!! ERROR LOADING GAME !!!" << std::endl;
      std::cout << "Save file not found: " << e.filename << std::endl;
      std::cout << "Starting new game..." << std::endl;
      waitForEnter();
    } catch (const InvalidSaveFileException &e) {
      std::cout << "\n!!! ERROR LOADING GAME !!!" << std::endl;
      std::cout << "Invalid or corrupted save file: " << e.what() << std::endl;
      std::cout << "Starting new game..." << std::endl;
      waitForEnter();
    } catch (const LoadException &e) {
      std::cout << "\n!!! ERROR LOADING GAME !!!" << std::endl;
      std::cout << "Error loading game: " << e.what() << std::endl;
      std::cout << "Starting new game..." << std::endl;
      waitForEnter();
    } catch (const GameException &e) {
      std::cout << "\n!!! ERROR LOADING GAME !!!" << std::endl;
      std::cout << "Error loading game: " << e.what() << std::endl;
      std::cout << "Starting new game..." << std::endl;
      waitForEnter();
    } catch (const std::exception &e) {
      std::cout << "\n!!! UNEXPECTED ERROR LOADING GAME !!!" << std::endl;
      std::cout << "Error: " << e.what() << std::endl;
      std::cout << "Starting new game..." << std::endl;
      waitForEnter();
    } catch (...) {
      std::cout << "\n!!! UNKNOWN ERROR LOADING GAME !!!" << std::endl;
      std::cout << "Starting new game..." << std::endl;
      waitForEnter();
    }
  }

  startLevel(1);
  runLevel(1);
}

void GameManager::runLevel(int level) {
  currentLevel = level;
  gameRunning = true;

  while (gameRunning && field && field->getPlayer() &&
         field->getPlayer()->isAlive()) {
    bool turnTaken = processPlayerTurn();

    if (!gameRunning) {
      break;
    }

    if (turnTaken) {
      processEnemyTurns();

      LevelResult result = updateGame();

      if (result == LevelResult::LEVEL_COMPLETE) {
        levelComplete();
        if (!gameRunning) {
          break;
        }
        currentLevel++;
        startLevel(currentLevel);
      } else if (result == LevelResult::PLAYER_DIED) {
        playerDied();
        break;
      }
    }
  }

  if (!field || !field->getPlayer() || !field->getPlayer()->isAlive()) {
    playerDied();
  }
}

void GameManager::startLevel(int level) {
  currentLevel = level;
  int fieldSize = getLevelFieldSize(level);

  std::cout << "\n=== Starting Level " << level << " ===" << std::endl;
  std::cout << "Field size: " << fieldSize << "x" << fieldSize << std::endl;
  waitForEnter();

  field = std::make_unique<GameField>(fieldSize, fieldSize);

  if (hasSavedPlayerState) {
    applySavedPlayerState();
  }
  capturePlayerState();

  displayLevelInfo();
}

LevelResult GameManager::checkLevelCompletion() const {
  if (!field || !field->getPlayer()) {
    return LevelResult::PLAYER_DIED;
  }

  if (!field->getPlayer()->isAlive()) {
    return LevelResult::PLAYER_DIED;
  }

  if (field->getEnemies().empty() && field->getBuildings().empty() &&
      field->getTowers().empty()) {
    return LevelResult::LEVEL_COMPLETE;
  }

  return LevelResult::IN_PROGRESS;
}

bool GameManager::processPlayerTurn() {
  if (!field || !field->getPlayer()) {
    return false;
  }

  Player *player = field->getPlayer();

  if (player->getSlowedNextTurn()) {
    player->setSlowed(true);
    player->setSlowedNextTurn(false);
  }

  field->display();
  displayLevelInfo();
  printHelp();

  std::cout << "\nEnter command: ";

  if (!inputReader) {
    std::cerr << "Error: InputReader not initialized!" << std::endl;
    return false;
  }

  std::unique_ptr<Command> command = inputReader->readCommand();

  if (!command) {
    return false;
  }

  
  bool turn_taken = command->execute(this);

  return turn_taken;
}

void GameManager::processEnemyTurns() {
  processEnemyTurn();
  processBuildingsTurn();
  processTowersTurn();

  if (field->getPlayer() && field->getPlayer()->getIsSlowed()) {
    field->getPlayer()->setSlowed(false);
  }
}

LevelResult GameManager::updateGame() {
  LevelResult result = checkLevelCompletion();

  if (result == LevelResult::LEVEL_COMPLETE) {
    levelComplete();
    return LevelResult::LEVEL_COMPLETE;
  } else if (result == LevelResult::PLAYER_DIED) {
    playerDied();
    return LevelResult::PLAYER_DIED;
  }

  return LevelResult::IN_PROGRESS;
}

bool GameManager::handlePlayerMove(int dx, int dy) {
  if (!field)
    return false;
  bool turn_taken = field->movePlayer(dx, dy);
  if (!turn_taken && field->getPlayer() && field->getPlayer()->getIsSlowed()) {
    std::cout << "You are slowed and cannot move this turn!" << std::endl;
    turn_taken = true;
  }
  return turn_taken;
}

bool GameManager::handlePlayerAttack(int dx, int dy) {
  if (!field)
    return false;
  bool turn_taken = field->attackEnemy(dx, dy);
  if (!turn_taken) {
    std::cout << "No target in range!" << std::endl;
  }
  return turn_taken;
}

bool GameManager::handlePlayerCast() {
  if (!field || !field->getPlayer())
    return false;

  Player *player = field->getPlayer();
  PlayerHand *hand = player->getHand();

  if (!hand || hand->size() == 0) {
    std::cout << "No spells in hand." << std::endl;
    return false;
  }

  const auto &spells = hand->getSpells();
  std::cout << "Spells:" << std::endl;
  for (int i = 0; i < (int)spells.size(); ++i) {
    std::cout << i << ") " << spells[i]->name() << std::endl;
  }
  std::cout << "Choose index and target x y: ";

  int idx, tx, ty;
  if (std::cin >> idx >> tx >> ty) {
    if (hand->castAt(*field, *player, idx, tx, ty)) {
      std::cout << "Spell cast." << std::endl;
      return true;
    } else {
      std::cout << "Spell failed." << std::endl;
      return false;
    }
  } else {
    std::cin.clear();
    std::string dummy;
    std::getline(std::cin, dummy);
    std::cout << "Invalid input." << std::endl;
    return false;
  }
}

void GameManager::processEnemyTurn() {
  if (!field)
    return;
  field->removeDeadEnemies();
  field->moveEnemies();
}

void GameManager::processBuildingsTurn() {
  if (!field)
    return;
  field->updateBuildings();
}

void GameManager::processTowersTurn() {
  if (!field)
    return;
  field->updateTowers();
}

void GameManager::levelComplete() {
  std::cout << "\n=== Level " << currentLevel << " Complete! ===" << std::endl;

  Player *player = field->getPlayer();
  if (player) {
    std::cout << "Final Score: " << player->getScore() << std::endl;

    
    if (EventSystem::getInstance().isLoggingEnabled()) {
      LevelCompleteEvent event(currentLevel, player->getScore());
      EventSystem::getInstance().notifyEvent(event);
    }
  }

  if (player) {
    int currentHp = player->getHp();
    int maxHp = std::max(currentHp, PLAYER_DEFAULT_HP);
    player->setHp(maxHp);
    std::cout << "HP restored to " << maxHp << std::endl;
  }

  if (player && player->getHand()) {
    removeHalfSpells(player);
  }

  showUpgradeMenu();
  capturePlayerState();

  std::cout << "\nPress Enter to continue to next level...";
  std::cin.ignore();
  std::cin.get();
}

void GameManager::playerDied() {
  std::cout << "\n=== Game Over! You Died! ===" << std::endl;

  if (field && field->getPlayer()) {
    std::cout << "Final Score: " << field->getPlayer()->getScore() << std::endl;
    std::cout << "Level Reached: " << currentLevel << std::endl;

    
    if (EventSystem::getInstance().isLoggingEnabled()) {
      PlayerDeathEvent event(currentLevel, field->getPlayer()->getScore());
      EventSystem::getInstance().notifyEvent(event);
    }
  }

  if (askToRestart()) {
    startGame();
  } else {
    gameRunning = false;
  }
}

void GameManager::removeHalfSpells(Player *player) {
  if (!player || !player->getHand())
    return;

  PlayerHand *hand = player->getHand();
  int spellCount = hand->size();
  int toRemove = spellCount / 2;

  if (toRemove == 0) {
    std::cout << "No spells to remove." << std::endl;
    return;
  }

  std::random_device rd;
  std::mt19937 rng(rd());
  std::vector<int> indices;
  for (int i = 0; i < spellCount; ++i) {
    indices.push_back(i);
  }
  std::shuffle(indices.begin(), indices.end(), rng);

  std::sort(indices.begin(), indices.begin() + toRemove, std::greater<int>());
  for (int i = 0; i < toRemove; ++i) {
    if (indices[i] < hand->size()) {
      hand->removeSpell(indices[i]);
    }
  }

  std::cout << "Removed " << toRemove << " spell(s) from your hand."
            << std::endl;
}

void GameManager::capturePlayerState() {
  if (!field || !field->getPlayer())
    return;

  Player *player = field->getPlayer();
  savedPlayerHp = player->getHp();
  savedPlayerBaseDamage = player->getBaseDamage();
  savedPlayerScore = player->getScore();
  savedAttackType = player->getAttackType();

  if (player->getHand()) {
    savedPlayerHandCapacity = player->getHand()->getCapacity();
    savedSpells.clear();
    const auto &spells = player->getHand()->getSpells();
    for (auto *spell : spells) {
      savedSpells.push_back(spell->name());
    }
  } else {
    savedPlayerHandCapacity = PLAYER_HAND_CAPACITY;
    savedSpells.clear();
  }
  hasSavedPlayerState = true;
}

void GameManager::applySavedPlayerState() {
  if (!hasSavedPlayerState || !field || !field->getPlayer())
    return;

  Player *player = field->getPlayer();
  player->setHp(savedPlayerHp);
  player->setBaseDamage(savedPlayerBaseDamage);
  player->setScore(savedPlayerScore);

  if (player->getAttackType() != savedAttackType) {
    player->switchAttackType();
  }

  PlayerHand *hand = player->getHand();
  if (hand) {
    hand->setCapacity(savedPlayerHandCapacity);
    hand->clear();
    for (const auto &spellName : savedSpells) {
      Spell *spell = createSpellByName(spellName);
      if (spell) {
        if (!hand->addSpell(spell)) {
          delete spell;
        }
      }
    }
    if (hand->size() == 0) {
      hand->addRandomSpell();
    }
  }
}

Spell *GameManager::createSpellByName(const std::string &name) {
  if (name == "Direct") {
    return makeDirectSpell();
  }
  if (name == "AoE") {
    return makeAreaSpell();
  }
  if (name == "Trap") {
    return makeTrapSpell();
  }
  return nullptr;
}

void GameManager::showUpgradeMenu() {
  if (!field || !field->getPlayer())
    return;

  Player *player = field->getPlayer();
  int score = player->getScore();

  std::cout << "\n=== Upgrade Menu ===" << std::endl;
  std::cout << "Current Score: " << score << std::endl;
  std::cout << "Current HP: " << player->getHp() << std::endl;
  std::cout << "Current Damage: " << player->getBaseDamage() << std::endl;
  std::cout << "Current Hand Capacity: "
            << (player->getHand() ? player->getHand()->getCapacity() : 0)
            << std::endl;
  std::cout << "\nAvailable Upgrades:" << std::endl;
  std::cout << "1. Increase HP (+50) - Cost: " << UPGRADE_HP_COST << " points"
            << std::endl;
  std::cout << "2. Increase Damage (+5) - Cost: " << UPGRADE_DMG_COST
            << " points" << std::endl;
  std::cout << "3. Increase Hand Capacity (+1) - Cost: " << UPGRADE_HAND_COST
            << " points" << std::endl;
  std::cout << "0. Skip upgrades" << std::endl;
  std::cout << "\nEnter choice: ";

  char choice;
  std::cin >> choice;

  if (choice >= '1' && choice <= '3') {
    applyUpgrade(choice);
  } else {
    std::cout << "Skipping upgrades." << std::endl;
  }
}

void GameManager::applyUpgrade(char choice) {
  if (!field || !field->getPlayer())
    return;

  Player *player = field->getPlayer();
  int score = player->getScore();

  switch (choice) {
  case '1':
    if (score >= UPGRADE_HP_COST) {
      player->setHp(player->getHp() + 50);
      player->addScore(-UPGRADE_HP_COST);
      std::cout << "HP increased by 50! New HP: " << player->getHp()
                << std::endl;
    } else {
      std::cout << "Not enough points! Need " << UPGRADE_HP_COST << ", have "
                << score << std::endl;
    }
    break;
  case '2':
    if (score >= UPGRADE_DMG_COST) {
      player->setBaseDamage(player->getBaseDamage() + 5);
      player->addScore(-UPGRADE_DMG_COST);
      std::cout << "Damage increased by 5! New base damage: "
                << player->getBaseDamage() << std::endl;
    } else {
      std::cout << "Not enough points! Need " << UPGRADE_DMG_COST << ", have "
                << score << std::endl;
    }
    break;
  case '3':
    if (score >= UPGRADE_HAND_COST && player->getHand()) {
      int newCapacity = player->getHand()->getCapacity() + 1;
      player->getHand()->setCapacity(newCapacity);
      player->addScore(-UPGRADE_HAND_COST);
      std::cout << "Hand capacity increased by 1! New capacity: " << newCapacity
                << std::endl;
    } else {
      if (score < UPGRADE_HAND_COST) {
        std::cout << "Not enough points! Need " << UPGRADE_HAND_COST
                  << ", have " << score << std::endl;
      }
    }
    break;
  }
}

GameCommand GameManager::parseCommand(const std::string &cmd) {
  static const std::unordered_map<std::string, GameCommand> commandMap = {
      {"w", GameCommand::MOVE_UP},
      {"a", GameCommand::MOVE_LEFT},
      {"s", GameCommand::MOVE_DOWN},
      {"d", GameCommand::MOVE_RIGHT},
      {"h", GameCommand::ATTACK_LEFT},
      {"j", GameCommand::ATTACK_UP},
      {"k", GameCommand::ATTACK_RIGHT},
      {"l", GameCommand::ATTACK_DOWN},
      {"switch", GameCommand::SWITCH_ATTACK},
      {"cast", GameCommand::CAST_SPELL},
      {"save", GameCommand::SAVE},
      {"skip", GameCommand::SKIP},
      {"help", GameCommand::HELP},
      {"quit", GameCommand::QUIT}};

  auto it = commandMap.find(cmd);
  return (it != commandMap.end()) ? it->second : GameCommand::UNKNOWN;
}

void GameManager::printHelp() {
  std::cout << "\nCommands:" << std::endl;

  if (inputReader) {
    const KeyBindings &bindings = inputReader->getBindings();

    
    std::cout << bindings.getKey(CommandType::MOVE_UP) << "/"
              << bindings.getKey(CommandType::MOVE_LEFT) << "/"
              << bindings.getKey(CommandType::MOVE_DOWN) << "/"
              << bindings.getKey(CommandType::MOVE_RIGHT)
              << " - move up/left/down/right" << std::endl;

    
    std::cout << bindings.getKey(CommandType::ATTACK_LEFT) << "/"
              << bindings.getKey(CommandType::ATTACK_UP) << "/"
              << bindings.getKey(CommandType::ATTACK_RIGHT) << "/"
              << bindings.getKey(CommandType::ATTACK_DOWN)
              << " - attack left/up/right/down" << std::endl;

    
    std::cout << bindings.getKey(CommandType::SWITCH_ATTACK)
              << " - switch attack type" << std::endl;
    std::cout << bindings.getKey(CommandType::CAST_SPELL) << " - cast a spell"
              << std::endl;
    std::cout << bindings.getKey(CommandType::SAVE) << " - save game"
              << std::endl;
    std::cout << bindings.getKey(CommandType::SKIP) << " - skip turn"
              << std::endl;
    std::cout << bindings.getKey(CommandType::HELP) << " - show this help"
              << std::endl;
    std::cout << bindings.getKey(CommandType::QUIT) << " - exit game"
              << std::endl;
  } else {
    
    std::cout << "w/a/s/d - move up/left/down/right" << std::endl;
    std::cout << "h/j/k/l - attack left/up/right/down" << std::endl;
    std::cout << "switch - switch attack type" << std::endl;
    std::cout << "cast - cast a spell" << std::endl;
    std::cout << "save - save game" << std::endl;
    std::cout << "skip - skip turn" << std::endl;
    std::cout << "help - show this help" << std::endl;
    std::cout << "quit - exit game" << std::endl;
  }
}

void GameManager::displayLevelInfo() const {
  if (!field)
    return;
  std::cout << "\n=== Level " << currentLevel << " ===" << std::endl;
  std::cout << "Enemies: " << field->getEnemies().size() << std::endl;
  std::cout << "Buildings: " << field->getBuildings().size() << std::endl;
  std::cout << "Towers: " << field->getTowers().size() << std::endl;
}

void GameManager::handleSave() {
  try {
    if (!field || !field->getPlayer()) {
      std::cout << "Cannot save: No game in progress." << std::endl;
      return;
    }

    std::cout << "\n=== Saving Game ===" << std::endl;
    std::cout << "Level: " << currentLevel << std::endl;

    Player *player = field->getPlayer();
    if (player) {
      std::cout << "Player HP: " << player->getHp() << std::endl;
      std::cout << "Player Score: " << player->getScore() << std::endl;
      std::cout << "Player Position: (" << player->getX() << ", "
                << player->getY() << ")" << std::endl;
      if (player->getHand()) {
        std::cout << "Spells in hand: " << player->getHand()->size()
                  << std::endl;
      }
    }

    GameState state(currentLevel, field->getPlayer(), field.get());
    std::string filename = GameSaveLoad::DEFAULT_SAVE_FILE;
    saveGame(filename);

    std::cout << "Game saved successfully to: " << filename << std::endl;
    std::cout << "Saved data:" << std::endl;
    std::cout << "  - Level: " << state.currentLevel << std::endl;
    std::cout << "  - HP: " << state.playerHp << std::endl;
    std::cout << "  - Base Damage: " << state.playerBaseDmg << std::endl;
    std::cout << "  - Score: " << state.playerScore << std::endl;
    std::cout << "  - Hand Capacity: " << state.playerHandCapacity << std::endl;
    std::cout << "  - Spells: " << state.playerSpells.size() << " ("
              << (state.playerSpells.empty() ? "none" : "") << ")";
    if (!state.playerSpells.empty()) {
      for (size_t i = 0; i < state.playerSpells.size(); ++i) {
        std::cout << (i == 0 ? " " : ", ") << state.playerSpells[i];
      }
    }
    std::cout << std::endl;
    std::cout << "  - Field Size: " << state.fieldWidth << "x"
              << state.fieldHeight << std::endl;

    if (GameSaveLoad::saveFileExists(filename)) {
      std::cout << "File verification: " << filename
                << " exists and is readable" << std::endl;
    } else {
      std::cout << "WARNING: File " << filename
                << " was not found after saving!" << std::endl;
    }
    std::cout << "=== Save Complete ===" << std::endl;
  } catch (const SaveException &e) {
    std::cout << "\n!!! ERROR SAVING GAME !!!" << std::endl;
    std::cout << "Error: " << e.what() << std::endl;
    std::cout << "File: " << e.filename << std::endl;
    std::cout << "Game was NOT saved!" << std::endl;
    waitForEnter();
  } catch (const std::exception &e) {
    std::cout << "\n!!! UNEXPECTED ERROR WHILE SAVING !!!" << std::endl;
    std::cout << "Error: " << e.what() << std::endl;
    std::cout << "Game was NOT saved!" << std::endl;
    waitForEnter();
  } catch (...) {
    std::cout << "\n!!! UNKNOWN ERROR WHILE SAVING !!!" << std::endl;
    std::cout << "Game was NOT saved!" << std::endl;
    waitForEnter();
  }
}

void GameManager::handleLoad() {
  try {
    std::cout << "\n=== Loading Game ===" << std::endl;
    loadGame();
    std::cout << "Game loaded successfully!" << std::endl;
    std::cout << "=== Load Complete ===" << std::endl;
  } catch (const FileNotFoundException &e) {
    std::cout << "\n!!! ERROR LOADING GAME !!!" << std::endl;
    std::cout << "Save file not found: " << e.filename << std::endl;
    std::cout << "Game was NOT loaded!" << std::endl;
    waitForEnter();
  } catch (const InvalidSaveFileException &e) {
    std::cout << "\n!!! ERROR LOADING GAME !!!" << std::endl;
    std::cout << "Invalid or corrupted save file: " << e.what() << std::endl;
    std::cout << "File: " << e.filename << std::endl;
    std::cout << "Game was NOT loaded!" << std::endl;
    waitForEnter();
  } catch (const LoadException &e) {
    std::cout << "\n!!! ERROR LOADING GAME !!!" << std::endl;
    std::cout << "Error loading game: " << e.what() << std::endl;
    std::cout << "File: " << e.filename << std::endl;
    std::cout << "Game was NOT loaded!" << std::endl;
    waitForEnter();
  } catch (const std::exception &e) {
    std::cout << "\n!!! UNEXPECTED ERROR LOADING GAME !!!" << std::endl;
    std::cout << "Error: " << e.what() << std::endl;
    std::cout << "Game was NOT loaded!" << std::endl;
    waitForEnter();
  } catch (...) {
    std::cout << "\n!!! UNKNOWN ERROR LOADING GAME !!!" << std::endl;
    std::cout << "Game was NOT loaded!" << std::endl;
    waitForEnter();
  }
}

bool GameManager::askToLoad() {
  if (!hasSaveFile()) {
    return false;
  }

  std::cout << "Save file found. Load game? (y/n): ";
  char choice;
  std::cin >> choice;
  return choice == 'y' || choice == 'Y';
}

bool GameManager::askToRestart() {
  std::cout << "\nRestart game? (y/n): ";
  char choice;
  std::cin >> choice;
  return choice == 'y' || choice == 'Y';
}

void GameManager::saveGame(const std::string &filename) {
  if (!field || !field->getPlayer()) {
    throw SaveException(filename, "No game state to save");
  }

  GameState state(currentLevel, field->getPlayer(), field.get());
  GameSaveLoad::saveGame(state, filename);
}

void GameManager::loadGame(const std::string &filename) {
  GameState state = GameSaveLoad::loadGame(filename);

  currentLevel = state.currentLevel;

  field = std::make_unique<GameField>(state.fieldWidth, state.fieldHeight);

  Player *player = field->getPlayer();
  if (player) {
    player->setHp(state.playerHp);
    player->setBaseDamage(state.playerBaseDmg);
    player->setScore(state.playerScore);

    if (player->getHand()) {
      player->getHand()->setCapacity(state.playerHandCapacity);
      player->getHand()->clear();

      for (const auto &spellName : state.playerSpells) {
        Spell *spell = nullptr;
        if (spellName == "Direct") {
          spell = makeDirectSpell();
        } else if (spellName == "AoE") {
          spell = makeAreaSpell();
        } else if (spellName == "Trap") {
          spell = makeTrapSpell();
        }

        if (spell) {
          player->getHand()->addSpell(spell);
        }
      }
    }
  }
  capturePlayerState();
}

bool GameManager::hasSaveFile(const std::string &filename) const {
  return GameSaveLoad::saveFileExists(filename);
}


bool GameManager::executeMove(int dx, int dy) {
  return handlePlayerMove(dx, dy);
}

bool GameManager::executeAttack(int dx, int dy) {
  return handlePlayerAttack(dx, dy);
}

bool GameManager::executeSwitchAttack() {
  if (!field || !field->getPlayer())
    return false;
  field->getPlayer()->switchAttackType();
  std::cout << "Attack type switched!" << std::endl;
  return true;
}

bool GameManager::executeCastSpell() { return handlePlayerCast(); }

void GameManager::executeSave() { handleSave(); }

void GameManager::executeHelp() { printHelp(); }

void GameManager::executeQuit() { gameRunning = false; }
