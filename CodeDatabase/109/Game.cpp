#include "Game.h"
#include "GameLevel.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <sstream>

Game::Game(int width, int height, int startLevel) 
  : map(width, height), player(), running(true), 
    currentLevel(startLevel), currentGameLevel(startLevel),
    hasSavedGame(false), gameLoaded(false) {  
}

void Game::processGameLoop() {
  if (!player.isAlive()) {
    GameOver();
    return;
  }
  
  // в GameController
  playerTurn();
  
  // Если игрок мертв после своего хода
  if (!player.isAlive()) {
    GameOver();
    return;
  }
  
  alliesTurn();
  enemiesTurn();
  towersTurn();
  
  if (checkLevelCompletion()) {
    handleLevelTransition();
  }
  player.addScore(1);
  player.addMana(1);
}

void Game::initializeLevel() {
  GameLevel level(currentLevel);

  enemies.clear();
  towers.clear();
  traps.clear();
  allies.clear();

  if (!gameLoaded) {
    // Сохраняем улучшенные характеристики
    int currentMaxHealth = player.getMaxHealth();
    int currentMeleeDamage = player.getMeleeDamage();
    int currentRangedDamage = player.getRangedDamage();
    int currentMaxMana = player.getMaxMana();
    
    player = Player(currentMaxHealth, currentMeleeDamage, currentRangedDamage, 
                    currentMaxMana, currentMaxMana);
  }
  
  int centerX = map.getWidth() / 2;
  int centerY = map.getHeight() / 2;
  player.setPos(centerX, centerY);
  
  for (int y = 0; y < map.getHeight(); ++y) {
    for (int x = 0; x < map.getWidth(); ++x) {
      CellType cell = map.getCell(x, y);
      if (cell == CellType::PLAYER || cell == CellType::ENEMY || 
        cell == CellType::TOWER || cell == CellType::TRAP || 
        cell == CellType::ALLY) {
        map.clearCell(x, y);
      }
    }
  }
  
  map.setCell(centerX, centerY, CellType::PLAYER);
  
  // Спавним врагов и башни согласно уровню
  spawnEnemies(level.getEnemyCount());
  spawnTowers(level.getTowerCount());
  
  gameLoaded = false;
  
  std::cout << "\n*** STARTING LEVEL " << currentLevel << " ***" << std::endl;
  std::cout << "Map size: " << map.getWidth() << "x" << map.getHeight() << std::endl;
  std::cout << "Enemies: " << level.getEnemyCount() << std::endl;
  if (currentLevel > 1) {
    std::cout << "Towers: " << level.getTowerCount() << std::endl;
  }
}

void Game::spawnEnemies(int count) {
  for (int i = 0; i < count; ++i) {
    int x, y;
    int attempts = 0;
    do {
      x = rand() % map.getWidth();
      y = rand() % map.getHeight();
      attempts++;
    } while ((!map.isEmpty(x, y) || map.isWall(x, y) || map.getCell(x, y) == CellType::TOWER ||
              abs(x - player.getPos().getX()) + abs(y - player.getPos().getY()) < 3) && attempts < 100);
        
    if (attempts < 100) {
        Enemy enemy(currentGameLevel.getEnemyHealth(), currentGameLevel.getEnemyDamage());
        enemy.setPos(x, y);
        enemies.push_back(enemy);
        map.setCell(x, y, CellType::ENEMY);
    }
  }
}

void Game::spawnTowers(int count) {
  for (int i = 0; i < count; ++i) {
    int x, y;
    int attempts = 0;
    do {
      x = rand() % map.getWidth();
      y = rand() % map.getHeight();
      attempts++;
    } while ((!map.isEmpty(x, y) || map.isWall(x, y) || 
              abs(x - player.getPos().getX()) + abs(y - player.getPos().getY()) < 5) &&
              attempts < 100);
    
    if (attempts < 100) {
      EnemyTower tower(x, y);
      towers.push_back(tower);
      map.setCell(x, y, CellType::TOWER);
    }
  }
}

void Game::playerTurn() {
  std::cout << "\n=== YOUR TURN ===" << std::endl;
  std::cout << "Enter command (see bindings above): " << std::endl;
}

void Game::alliesTurn() {
  if (allies.empty()) return;
  
  std::cout << "=== ALLIES TURN ===" << std::endl;
  updateAllies();
}

void Game::enemiesTurn() {
  if (enemies.empty()) return;
  
  std::cout << "=== ENEMIES TURN ===" << std::endl;
  updateEnemies();
}

void Game::towersTurn() {
  if (towers.empty()) return;
  
  std::cout << "=== TOWERS TURN ===" << std::endl;
  for (auto& tower : towers) {
    if (tower.isAlive() && player.getInvisibilityTurns() == 0 && tower.canAttack(player.getPos())) {
      tower.attack(player);
    }
    tower.updateCooldown();
  }
}

bool Game::checkLevelCompletion() {
  // Уровень 1: только враги
  if (currentLevel == 1) {
    for (const auto& enemy : enemies) {
      if (enemy.isAlive()) return false;
    }
    return true;
  }
  
  for (const auto& enemy : enemies) {
    if (enemy.isAlive()) return false;
  }
  for (const auto& tower : towers) {
    if (tower.isAlive()) return false;
  }
  return true;
}

void Game::handleLevelTransition() {
  std::cout << "\n*** LEVEL " << currentLevel << " COMPLETED! ***" << std::endl;
  std::cout << "Final score for this level: " << player.getScore() << std::endl;
  
  if (currentLevel < 5) {
    currentLevel++;

    enemies.clear();
    towers.clear();
    traps.clear();
    allies.clear();

    GameLevel nextLevel(currentLevel);
    map = GameMap(nextLevel.getMapWidth(), nextLevel.getMapHeight());
    showLevelUpMenu();
    initializeLevel();
  } else {
    std::cout << "*** CONGRATULATIONS! YOU BEAT THE GAME! ***" << std::endl;
    running = false;
  }
}

void Game::showLevelUpMenu() {
  int choice;
  
  while (true) {
    std::cout << "\n=== LEVEL UP! Choose upgrade ===" << std::endl;
    std::cout << "1. Magic Power (+10% max mana, +10% spell power)" << std::endl;
    std::cout << "2. Strength (+10% physical damage)" << std::endl;
    std::cout << "Choice (1-2): ";
    
    if (std::cin >> choice) {
      if (choice == 1 || choice == 2) {
        break; 
      }
    } else {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
    }
    std::cout << "Invalid input! Please enter 1 or 2." << std::endl;
  }
  
  switch (choice) {
    case 1:
      applyMagicUpgrade();
      break;
    case 2:
      applyStrengthUpgrade();
      break;
    default:
      std::cout << "Invalid choice! Magic power selected by default." << std::endl;
      applyMagicUpgrade();
  }
  
  // Удаляем половину случайных заклинаний
  int spellsToRemove = player.getSpellHand().getSpellCount() / 2;
  removeRandomSpells(spellsToRemove);
  
  player.setHealth(player.getMaxHealth());
  player.addMana(player.getMaxMana() - player.getMana());
}

void Game::applyMagicUpgrade() {
  int newMaxMana = static_cast<int>(player.getMaxMana() * 1.1);
  player.setMaxMana(newMaxMana);
  player.addMana(newMaxMana - player.getMana());
  
  std::cout << "Magic power increased! Max mana: " << newMaxMana << std::endl;
}

void Game::applyStrengthUpgrade() {
  int newMeleeDamage = static_cast<int>(player.getMeleeDamage() * 1.1);
  int newRangedDamage = static_cast<int>(player.getRangedDamage() * 1.1);  
  player.setMeleeDamage(newMeleeDamage);
  player.setRangedDamage(newRangedDamage);
  
  std::cout << "Strength increased! Melee: " << newMeleeDamage 
          << ", Ranged: " << newRangedDamage << std::endl;
}

void Game::removeRandomSpells(int count) {
  for (int i = 0; i < count && player.getSpellHand().getSpellCount() > 0; ++i) {
    int randomIndex = rand() % player.getSpellHand().getSpellCount();
    player.getSpellHand().removeSpell(randomIndex);
  }
  std::cout << "Removed " << count << " random spells from your hand." << std::endl;
}

void Game::GameOver() {
  std::cout << "\n*** GAME OVER! ***" << std::endl;
  std::cout << "Final Score: " << player.getScore() << std::endl;
  std::cout << "Reached Level: " << currentLevel << std::endl;
  
  int choice;
  while (true) {
    std::cout << "\n1. Start New Game" << std::endl;
    if (hasSavedGame) {
      std::cout << "2. Load Saved Game" << std::endl;
    }
    std::cout << (hasSavedGame ? "3" : "2") << ". Exit" << std::endl;
    std::cout << "Choice (1-" << (hasSavedGame ? "3" : "2") << "): ";
    
    if (std::cin >> choice) {
      if (hasSavedGame) {
        if (choice >= 1 && choice <= 3) break;
      } else {
        if (choice == 1 || choice == 2) break;
      }
    }
    
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    std::cout << "Invalid input! Please try again." << std::endl;
  }
  
  switch (choice) {
    case 1:
      startNewGame();
      break;
    case 2:
      if (hasSavedGame) {
        loadGame();
      } else {
        running = false;
      }
      break;
    case 3:
      if (hasSavedGame) {
        running = false;
      }
    break;
  }
}

void Game::showSaveLoadMenu() {
  int choice;
  do {
    std::cout << "\n=== SAVE/LOAD MENU ===" << std::endl;
    std::cout << "1. Save Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Return to Game" << std::endl;
    std::cout << "Choose: ";
    
    if (std::cin >> choice) {
      switch (choice) {
        case 1:
          saveGame();
          break;
        case 2:
          loadGame();
          break;
        case 3:
          std::cout << "Returning to game..." << std::endl;
          break;
        default:
          std::cout << "Invalid choice! Please enter 1-3." << std::endl;
      }
    } else {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      std::cout << "Invalid input! Please enter a number." << std::endl;
    }
  } while (choice != 3);
}


void Game::saveGame() {
  try {
    gameSave.saveGame(*this);
    hasSavedGame = true;
  }
  catch (const GameSave::SaveException& e) {
    std::cout << e.what() << std::endl;
    std::cout << "Failed to save game. Continuing..." << std::endl;
  }
  catch (const std::exception& e) {
    std::cout << "Unexpected error during save: " << e.what() << std::endl;
    std::cout << "Continuing game..." << std::endl;
  }
}

void Game::loadGame() {
  try {
    if (!gameSave.saveExists()) {
      std::cout << "No saved game found!" << std::endl;
      return;
    }
      
    gameSave.loadGame(*this);
    hasSavedGame = true;
  }
  catch (const GameSave::LoadException& e) {
    std::cout << e.what() << std::endl;
    std::cout << "Starting new game instead..." << std::endl;
    startNewGame();
  }
  catch (const std::exception& e) {
    std::cout << "Unexpected error during load: " << e.what() << std::endl;
    std::cout << "Starting new game instead..." << std::endl;
    startNewGame();
  }
}

void Game::checkTraps() {
  for (auto it = traps.begin(); it != traps.end(); ) {
  bool trapTriggered = false;
  
  for (auto& enemy : enemies) {
    if (enemy.isAlive() && enemy.getPos() == *it) {
      enemy.takeDamage(5);
      std::cout << "Enemy triggered a trap! Damage: 5" << std::endl;
      trapTriggered = true;
      break;
    }
  }
  if (trapTriggered) {
    map.clearCell(it->getX(), it->getY()); 
    it = traps.erase(it);
  } else {
    ++it;
  }
  }
}

void Game::updateEnemies() {
  checkTraps();
  updateAllies();

  // обновляем башни
  for (size_t i = 0; i < towers.size(); ++i) {
    towers[i].updateCooldown();
    if (towers[i].isAlive()) {
      map.setCell(towers[i].getPos().getX(), towers[i].getPos().getY(), CellType::TOWER);
    }
  }
  
  // атаки башен
  for (size_t i = 0; i < towers.size(); ++i) {
    if (towers[i].isAlive() && player.getInvisibilityTurns() == 0 && towers[i].canAttack(player.getPos())) {
      towers[i].attack(player);
    }
  }

  // боработка врагов
  for (auto& enemy : enemies) {
    if (!enemy.isAlive()) continue;
    
    Position oldPos = enemy.getPos();
    map.clearCell(oldPos.getX(), oldPos.getY());
    
    int dx = abs(oldPos.getX() - player.getPos().getX());
    int dy = abs(oldPos.getY() - player.getPos().getY());
    int distance = dx + dy;
    
    if (player.getInvisibilityTurns() > 0) {
      enemy.makeMove(player.getPos(), map.getWidth(), map.getHeight());
    } else if (distance == 1) {
      player.takeDamage(enemy.getDamage());
      std::cout << "Enemy attacked you from adjacent cell! Damage: " << enemy.getDamage() 
          << " Health: " << player.getHealth() << std::endl;
      if (!player.isAlive()) {
        running = false;
        std::cout << "Game Over! Final score: " << player.getScore() << std::endl;
        return;
      }
    } else {
      enemy.makeMove(player.getPos(), map.getWidth(), map.getHeight());
    }
    
    Position newPos = enemy.getPos();

    // атака на союзников
    bool attackedAlly = false;
    for (auto& ally : allies) {
      if (ally.isAlive() && ally.getPos() == newPos) {
        ally.takeDamage(enemy.getDamage());
        std::cout << "Enemy attacked ally for " << enemy.getDamage() << " damage! ";
        std::cout << "Ally health: " << ally.getHealth() << std::endl;
        attackedAlly = true;
        enemy.setPos(oldPos.getX(), oldPos.getY());
        newPos = enemy.getPos();
        break;
      }
    }
    
    // проверяем можно ли встать
    if (map.isWall(newPos.getX(), newPos.getY()) || 
        map.getCell(newPos.getX(), newPos.getY()) == CellType::TOWER ||
        map.getCell(newPos.getX(), newPos.getY()) == CellType::PLAYER) {
      enemy.setPos(oldPos.getX(), oldPos.getY());
      newPos = enemy.getPos();
    }

    // отображаем врага
    if (map.isEmpty(newPos.getX(), newPos.getY()) || 
        map.getCell(newPos.getX(), newPos.getY()) == CellType::TRAP) {
      map.setCell(newPos.getX(), newPos.getY(), CellType::ENEMY);
    } else {
      enemy.setPos(oldPos.getX(), oldPos.getY());
      map.setCell(oldPos.getX(), oldPos.getY(), CellType::ENEMY);
    }
  }

  // отображаем игрока
  map.setCell(player.getPos().getX(), player.getPos().getY(), CellType::PLAYER);
  
  if (player.getInvisibilityTurns() > 0) {
    player.decrementInvisibility();
    if (player.getInvisibilityTurns() == 0) {
      std::cout << "Invisibility worn off!" << std::endl;
    }
  }
}

void Game::checkCollisions() {
  Position playerPos = player.getPos();
  for (auto& enemy : enemies) {
    if (!enemy.isAlive()) continue;
    
    if (enemy.getPos() == playerPos) {
      player.takeDamage(enemy.getDamage());
      std::cout << "Enemy hit you! Damage: " << enemy.getDamage() 
          << " Health: " << player.getHealth() << std::endl;
      
      if (!player.isAlive()) {
        running = false;
        std::cout << "Game Over! Final score: " << player.getScore() << std::endl;
        return; 
      }
    }
  }
}

void Game::render() {
  std::cout << "\n=== GAME ===" << std::endl;
  std::cout << "Health: " << player.getHealth() 
      << " | Score: " << player.getScore()
      << " | Mana: " << player.getMana() << "/" << player.getMaxMana();
  
  if (player.getMana() == 0) {
    std::cout << " [NO MANA]";
  }
  
  if (player.getInvisibilityTurns() > 0) {
    std::cout << " | INVISIBLE (" << player.getInvisibilityTurns() << " turns)";
  }

  if (player.getEnhancementPower() > 0) {
    std::cout << " | ENHANCE POWER: " << player.getEnhancementPower();
  }
  
  std::cout << std::endl;
  
  // Показываем заклинания
  std::cout << "Spells: ";
  for (int i = 0; i < player.getSpellHand().getSpellCount(); ++i) {
    Spell* spell = player.getSpellHand().getSpell(i);
    std::cout << (i+1) << "." << spell->getName() 
        << "(" << spell->getManaCost() << " mana) ";
  }
  std::cout << std::endl;
  
  map.display();
  std::cout << "============" << std::endl;
  //std::cout<< player.getEnhancementPower() << std::endl;
}

/*
void Game::processInput() {
  std::cout << "Move (w/a/s/d), Attack (m/r), Switch attack (t), Spells (1-5), Info (h), Quit (q), Menu(M): ";
  char input;
  std::cin >> input;
  
  if (input >= '1' && input <= '5') {
    processSpellInput(input);
    return;
  }
  
  if (input == 'h') {
    showSpellInfo();
    return;
  }

  if (input == 'M') {
    showSaveLoadMenu();
    return;
  }

  int dx = 0, dy = 0;
  bool moved = false;
  bool attacked = false;
  AttackType attackType;
  
  switch (input) {
    case 'w': dy = -1; moved = true; break;
    case 's': dy = 1; moved = true; break;
    case 'a': dx = -1; moved = true; break;
    case 'd': dx = 1; moved = true; break;
    case 'm': 
      attackType = AttackType::MELEE;
      attacked = true;
      break;
    case 'r': 
      attackType = AttackType::RANGED;
      attacked = true;
      break;
    case 't': 
      player.switchAttackType();
      std::cout << "Switched to " 
          << (player.getAttackType() == AttackType::MELEE ? "MELEE" : "RANGED")
          << " attack" << std::endl;
      return;
    case 'q': 
      running = false; 
      return;
    default: 
      std::cout << "Invalid command! Please use: w/a/s/d (move), m/r (attack), t (switch), 1-5 (spells), h (help), q (quit), M (save menu)" << std::endl;
      return;
  }
  
  if (moved) {
    Position current = player.getPos();
    int newX = current.getX() + dx;
    int newY = current.getY() + dy;
    
    if (map.inBounds(newX, newY) && !map.isWall(newX, newY) && map.isEmpty(newX, newY)) {
      map.clearCell(current.getX(), current.getY());
      player.move(dx, dy);
      map.setCell(newX, newY, CellType::PLAYER);
    }
  }
  
  if (attacked) {
    AttackType currentType = player.getAttackType();
    player.switchAttackType();

    bool hitSomething = false;
    
    // Сначала проверяем врагов
    for (size_t i = 0; i < enemies.size(); ++i) {
      if (enemies[i].isAlive() && player.canAttack(enemies[i].getPos(), attackType)) {
        enemies[i].takeDamage(player.getDamage());
        std::cout << "You hit enemy with " 
            << (attackType == AttackType::MELEE ? "MELEE" : "RANGED")
            << " attack for " << player.getDamage() << " damage! ";
      
        if (!enemies[i].isAlive()) {
          handleEnemyDefeat(i);
          hitSomething = true;
          break;
        } else {
          std::cout << "Enemy health: " << enemies[i].getHealth() << std::endl;
        }
        hitSomething = true;
        break;
      }
    }
    
    // Если не попали по врагу, проверяем башни
    if (!hitSomething) {
      for (size_t i = 0; i < towers.size(); ++i) {
        if (towers[i].isAlive() && player.canAttack(towers[i].getPos(), attackType)) {
          towers[i].takeDamage(player.getDamage());
          std::cout << "You hit tower with " 
              << (attackType == AttackType::MELEE ? "MELEE" : "RANGED")
              << " attack for " << player.getDamage() << " damage! ";
          
          if (!towers[i].isAlive()) {
            handleTowerDestruction(i);
            hitSomething = true;
            break;
          } else {
            std::cout << "Tower health: " << towers[i].getHealth() << std::endl;
          }
          hitSomething = true;
          break;
        }
      }
    }
    
    if (!hitSomething) {
      std::cout << "Attack missed!" << std::endl;
    }
    
    if (player.getAttackType() != currentType) {
      player.switchAttackType();
    }
  }
}
*/

void Game::startNewGame() {
  int width, height;

  while (true) {
    std::cout << "Enter map width (10-25): ";
    if (std::cin >> width) {
      if (width >= 10 && width <= 25) {
        break; 
      } else {
        std::cout << "Error: Width must be between 10 and 25." << std::endl;
      }
    } else {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      std::cout << "Error: Please enter a valid number between 10 and 25." << std::endl;
    }
  }
  while (true) {
    std::cout << "Enter map height (10-25): ";
    if (std::cin >> height) {
      if (height >= 10 && height <= 25) {
        break; 
      } else {
        std::cout << "Error: Height must be between 10 and 25." << std::endl;
      }
    } else {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      std::cout << "Error: Please enter a valid number between 10 and 25." << std::endl;
    }
  }
  
  // Создаем новую карту
  map = GameMap(width, height);
  
  currentLevel = 1;
  currentGameLevel = GameLevel(currentLevel);
  
  player = Player(100, 15, 10, 50, 50);
  
  enemies.clear();
  towers.clear();
  traps.clear();
  allies.clear();
  initializeLevel();
  
  std::cout << "New game started! Level " << currentLevel << std::endl;
}

void Game::processSpellInput(char input) {
  int spellIndex = input - '1';
  
  if (spellIndex < 0 || spellIndex >= player.getSpellHand().getSpellCount()) {
    std::cout << "Invalid spell selection!" << std::endl;
    return;
  }

  Spell* spell = player.getSpellHand().getSpell(spellIndex);

  if (player.getMana() < spell->getManaCost()) {
    std::cout << "Not enough mana! Need " << spell->getManaCost() 
      << ", have " << player.getMana() << std::endl;
    return;
  }

  if (spell->getType() == SpellType::ENHANCE) {
    player.accumulateEnhancement(); 
    std::cout << "Enhancement accumulated! Power: " << player.getEnhancementPower() << std::endl;
    std::cout << "Next non-Enhance spell will be boosted!" << std::endl;
    player.addMana(-spell->getManaCost());
    return;
  }
  
  // Автоматическое применение для некоторых заклинаний
  if (spell->getType() == SpellType::INVISIBILITY || spell->getType() == SpellType::SUMMON) {
    applySpellEffects(spell, player.getPos());
    player.addMana(-spell->getManaCost());
    return;
  }
  
  // Для остальных - запрашиваем направление
  std::cout << "Enter direction for " << spell->getName() << " (w/a/s/d): ";
  char direction;
  while (true) {
    std::cout << "Enter direction for " << spell->getName() << " (w/a/s/d): ";
    std::cin >> direction;
    
    if (direction == 'w' || direction == 'a' || direction == 's' || direction == 'd') {
      break;
    }
    std::cout << "Invalid direction! Please enter w, a, s, or d." << std::endl;
    
    std::cin.clear();
    std::cin.ignore(10000, '\n');
  }
  
  Position targetPos = player.getPos();
  int range = spell->getRange();
  
  switch (direction) {
    case 'w': targetPos.move(0, -range); break;  
    case 's': targetPos.move(0, range); break;   
    case 'a': targetPos.move(-range, 0); break;  
    case 'd': targetPos.move(range, 0); break;  
    default: 
      std::cout << "Invalid direction!" << std::endl;
      return;
  }
  
  if (map.inBounds(targetPos.getX(), targetPos.getY())) {
    applySpellEffects(spell, targetPos);
    player.addMana(-spell->getManaCost());
  } else {
    std::cout << "Target out of bounds!" << std::endl;
  }
}

void Game::handleEnemyDefeat(int enemyIndex) {
  if (enemyIndex < 0 || enemyIndex >= enemies.size()) {
    return;
  }
  
  Position enemyPos = enemies[enemyIndex].getPos();
  if (map.getCell(enemyPos.getX(), enemyPos.getY()) == CellType::ENEMY) {
    map.clearCell(enemyPos.getX(), enemyPos.getY());
  }
  player.addScore(10);
  std::cout << "Enemy defeated! +10 score" << std::endl;
  
  // Враги: 50% шанс
  if (rand() % 2 == 0) {
    giveReward(false); 
  } else {
    player.addMana(10);
    std::cout << "+10 mana" << std::endl;
  }
}

void Game::handleTowerDestruction(int towerIndex) {
  if (towerIndex < 0 || towerIndex >= towers.size()) {
    return;
  }
  
  map.clearCell(towers[towerIndex].getPos().getX(), towers[towerIndex].getPos().getY());
  player.addScore(20);
  std::cout << "Tower destroyed! +20 score" << std::endl;
  
  giveReward(true); 
}

void Game::giveReward(bool isTower) {
  if (player.getSpellHand().getSpellCount() < player.getSpellHand().getMaxSize()) {
    addRandomSpellToPlayer();
  } else {
    int manaAmount = isTower ? 15 : 10;
    player.addMana(manaAmount);
    std::cout << "Hand is full! +" << manaAmount << " mana instead" << std::endl;
  }
}

void Game::applySpellEffects(Spell* spell, const Position& targetPos) {
  int power = player.getEnhancementPower();
  
  int enhancedDamage = spell->getDamage() + (power * 3);
  int enhancedRange = spell->getRange() + power;
  int enhancedArea = spell->getArea() + power;
  
  switch (spell->getType()) {
    case SpellType::FIREBALL: {
      bool hit = false;
      
      for (size_t i = 0; i < enemies.size(); ++i) {
          if (!enemies[i].isAlive()) continue;
          
          int dx = abs(player.getPos().getX() - enemies[i].getPos().getX());
          int dy = abs(player.getPos().getY() - enemies[i].getPos().getY());
          int distance = dx + dy;
          
          if (distance <= enhancedRange && enemies[i].getPos() == targetPos) {
            bool wasAlive = enemies[i].isAlive();
            int oldHealth = enemies[i].getHealth();
            
            enemies[i].takeDamage(enhancedDamage);
            std::cout << "Fireball hit enemy for " << enhancedDamage << " damage!" << std::endl;
            
            if (wasAlive && !enemies[i].isAlive()) {
              handleEnemyDefeat(i);
            } else {
              std::cout << "Enemy health: " << enemies[i].getHealth() << std::endl;
            }
            hit = true;
            break;
          }
        }
        
        // Проверяем башни
        if (!hit) {
          for (size_t i = 0; i < towers.size(); ++i) {
            if (!towers[i].isAlive()) continue;
            
            int dx = abs(player.getPos().getX() - towers[i].getPos().getX());
            int dy = abs(player.getPos().getY() - towers[i].getPos().getY());
            int distance = dx + dy;
            
            if (distance <= enhancedRange && towers[i].getPos() == targetPos) {
              bool wasAlive = towers[i].isAlive();
              int oldHealth = towers[i].getHealth();
              
              towers[i].takeDamage(enhancedDamage);
              std::cout << "Fireball hit tower for " << enhancedDamage << " damage!" << std::endl;
              
              if (wasAlive && !towers[i].isAlive()) {
                handleTowerDestruction(i);
              } else {
                std::cout << "Tower health: " << towers[i].getHealth() << std::endl;
              }
              hit = true;
              break;
            }
          }
        }
        
        if (!hit) {
          std::cout << "Fireball missed!" << std::endl;
        }
        break;
    }
    
    case SpellType::EXPLOSION: {
      int hits = 0;
      
      int baseRadius = 1; 
      int enhancedRadius = baseRadius + power; 
      
      std::vector<int> killedEnemies;
      std::vector<int> killedTowers;
      
      for (int dx = -enhancedRadius; dx <= enhancedRadius; ++dx) {
        for (int dy = -enhancedRadius; dy <= enhancedRadius; ++dy) {
          // Пропускаем игрока
          if (dx == 0 && dy == 0) continue;
          
          int x = targetPos.getX() + dx;
          int y = targetPos.getY() + dy;
          
          if (map.inBounds(x, y)) {
            Position checkPos(x, y);
            
            // Враги
            for (size_t i = 0; i < enemies.size(); ++i) {
              if (enemies[i].isAlive() && enemies[i].getPos() == checkPos) {
                bool wasAlive = enemies[i].isAlive();
                enemies[i].takeDamage(enhancedDamage);
                hits++;
                
                if (wasAlive && !enemies[i].isAlive()) {
                  killedEnemies.push_back(i);
                  }
              }
            }
            
            // Башни
            for (size_t i = 0; i < towers.size(); ++i) {
              if (towers[i].isAlive() && towers[i].getPos() == checkPos) {
                bool wasAlive = towers[i].isAlive();
                towers[i].takeDamage(enhancedDamage);
                hits++;
                
                if (wasAlive && !towers[i].isAlive()) {
                  killedTowers.push_back(i);
                }
              }
            }
          }
        }
      }
      
      for (int index : killedEnemies) {
        handleEnemyDefeat(index);
      }
        
        for (int index : killedTowers) {
          handleTowerDestruction(index);
        }
        
        if (hits > 0) {
          std::cout << "Explosion hit " << hits << " targets in " 
                    << ((enhancedRadius * 2) + 1) << "x" << ((enhancedRadius * 2) + 1) 
                    << " area for " << enhancedDamage << " damage each!" << std::endl;
        } else {
          std::cout << "Explosion missed all targets!" << std::endl;
        }
        break;
      }

      case SpellType::TRAP: {
        if (map.isEmpty(targetPos.getX(), targetPos.getY())) {
          traps.push_back(targetPos);
          map.setCell(targetPos.getX(), targetPos.getY(), CellType::TRAP);
          std::cout << "Trap placed! Damage: " << enhancedDamage << std::endl;
        } else {
          std::cout << "Cannot place trap there!" << std::endl;
        }
        break;
      }
      
      case SpellType::INVISIBILITY: {
        int duration = 3 + power; 
        player.setInvisibilityTurns(duration);
        std::cout << "You are invisible for " << duration << " turns!" << std::endl;
        break;
      }
      
      case SpellType::SUMMON: {
        int summonCount = 1 + power; 
        std::cout << "Attempting to summon " << summonCount << " allies..." << std::endl;
        
        int successful = 0;
        for (int i = 0; i < summonCount; ++i) {
          if (summonAlly(player.getPos())) {
            successful++;
          }
        }
        
        std::cout << "Successfully summoned " << successful << " allies!" << std::endl;
        break;
      }
      
      case SpellType::ENHANCE: {
        break;
      }
  }
  
  // Сбрасываем улучшение
  if (spell->getType() != SpellType::ENHANCE && power > 0) {
    std::cout << "(Applied " << power << " enhancement levels)" << std::endl;
    player.resetEnhancement();
  }
}

void Game::addRandomSpellToPlayer() {
  if (player.getSpellHand().getSpellCount() >= player.getSpellHand().getMaxSize()) {
    player.addMana(10);
    std::cout << "Hand full! +10 mana" << std::endl;
    return;
  }

  for (int attempt = 0; attempt < 3; ++attempt) {
    Spell* newSpell = SpellHand::createRandomSpell();
    
    if (player.getSpellHand().addSpell(newSpell)) {
      std::cout << "Learned: " << newSpell->getName() << "!" << std::endl;
      return;
    } else {
      delete newSpell;
    }
  }
  
  player.addMana(15);
  std::cout << "Could not find new spell! +15 mana" << std::endl;
}

bool Game::summonAlly(const Position& pos) {\
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0) continue;
      
      int x = pos.getX() + dx;
      int y = pos.getY() + dy;
      
      if (map.inBounds(x, y) && map.isEmpty(x, y) && !map.isWall(x, y) && 
          map.getCell(x, y) != CellType::ENEMY && 
          map.getCell(x, y) != CellType::TOWER &&
          map.getCell(x, y) != CellType::ALLY) {
        Ally ally(x, y);
        allies.push_back(ally);
        map.setCell(x, y, CellType::ALLY);
        return true;
      }
    }
  }
  return false;
}

void Game::updateAllies() {
  for (auto it = allies.begin(); it != allies.end(); ) {
    if (!it->isAlive()) {
      std::cout << "Ally has fallen in battle!" << std::endl;
      map.clearCell(it->getPos().getX(), it->getPos().getY());
      it = allies.erase(it);
    } else {
      Position oldPos = it->getPos();
      map.clearCell(oldPos.getX(), oldPos.getY());
      
      bool attacked = false;
      for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
          int dx = abs(oldPos.getX() - enemy.getPos().getX());
          int dy = abs(oldPos.getY() - enemy.getPos().getY());
          int distance = dx + dy;
          
          if (distance == 1) {
            enemy.takeDamage(it->getDamage());
            std::cout << "Ally attacked enemy for " << it->getDamage() << " damage! ";
            if (!enemy.isAlive()) {
              std::cout << "Enemy defeated!";
              map.clearCell(enemy.getPos().getX(), enemy.getPos().getY());
            } else {
              std::cout << "Enemy health: " << enemy.getHealth();
            }
            std::cout << std::endl;
            attacked = true;
            break;
          }
        }
      }
      
      Position newPos = oldPos;
      if (!attacked) {
        it->makeMove(player.getPos(), map.getWidth(), map.getHeight());
        newPos = it->getPos();
        for (auto& enemy : enemies) {
          if (enemy.isAlive() && enemy.getPos() == newPos) {
            enemy.takeDamage(it->getDamage());
            std::cout << "Ally attacked enemy for " << it->getDamage() << " damage! ";
            if (!enemy.isAlive()) {
              std::cout << "Enemy defeated!";
              map.clearCell(enemy.getPos().getX(), enemy.getPos().getY());
            } else {
              std::cout << "Enemy health: " << enemy.getHealth();
            }
            std::cout << std::endl;
            attacked = true;
            it->setPos(oldPos.getX(), oldPos.getY());
            newPos = it->getPos();
            break;
          }
        }
      }
      
      // Можно ли встать
      if (!attacked && (map.isWall(newPos.getX(), newPos.getY()) || 
        map.getCell(newPos.getX(), newPos.getY()) == CellType::TOWER ||
        map.getCell(newPos.getX(), newPos.getY()) == CellType::PLAYER ||
        map.getCell(newPos.getX(), newPos.getY()) == CellType::ALLY)) {
        it->setPos(oldPos.getX(), oldPos.getY());
        newPos = it->getPos();
      }
      map.setCell(newPos.getX(), newPos.getY(), CellType::ALLY);
      ++it;
    }
  }
}

void Game::movePlayer(int dx, int dy) {
  Position current = player.getPos();
  int newX = current.getX() + dx;
  int newY = current.getY() + dy;
  
  if (map.inBounds(newX, newY) && !map.isWall(newX, newY) && map.isEmpty(newX, newY)) {
    map.clearCell(current.getX(), current.getY());
    player.move(dx, dy);
    map.setCell(newX, newY, CellType::PLAYER);
  }
}

void Game::playerAttack(AttackType type) {
  bool hitSomething = false;
  
  // Сначала проверяем врагов
  for (size_t i = 0; i < enemies.size(); ++i) {
    if (enemies[i].isAlive() && player.canAttack(enemies[i].getPos(), type)) {
      enemies[i].takeDamage(player.getDamage());
      std::cout << "You hit enemy with " 
        << (type == AttackType::MELEE ? "MELEE" : "RANGED")
        << " attack for " << player.getDamage() << " damage! ";
  
      if (!enemies[i].isAlive()) {
        handleEnemyDefeat(i);
      } else {
        std::cout << "Enemy health: " << enemies[i].getHealth() << std::endl;
      }
      hitSomething = true;
      break;
    }
  }
  
  // Если не попали по врагу, проверяем башни
  if (!hitSomething) {
    for (size_t i = 0; i < towers.size(); ++i) {
      if (towers[i].isAlive() && player.canAttack(towers[i].getPos(), type)) {
        towers[i].takeDamage(player.getDamage());
        std::cout << "You hit tower with " 
          << (type == AttackType::MELEE ? "MELEE" : "RANGED")
          << " attack for " << player.getDamage() << " damage! ";
        
        if (!towers[i].isAlive()) {
          handleTowerDestruction(i);
        } else {
          std::cout << "Tower health: " << towers[i].getHealth() << std::endl;
        }
        hitSomething = true;
        break;
      }
    }
  }
  
  if (!hitSomething) {
    std::cout << "Attack missed!" << std::endl;
  }
}

void Game::playerSwitchAttackType() {
  player.switchAttackType();
  std::cout << "Switched to " 
    << (player.getAttackType() == AttackType::MELEE ? "MELEE" : "RANGED")
    << " attack" << std::endl;
}

void Game::castSpell(int spellIndex) {
  if (spellIndex < 0 || spellIndex >= player.getSpellHand().getSpellCount()) {
    std::cout << "Invalid spell selection!" << std::endl;
    return;
  }
  
  Spell* spell = player.getSpellHand().getSpell(spellIndex);
  
  if (player.getMana() < spell->getManaCost()) {
    std::cout << "Not enough mana!" << std::endl;
    return;
  }
  applySpellEffects(spell, player.getPos());
  player.addMana(-spell->getManaCost());
}

void Game::updateGameState() {
  alliesTurn();
  enemiesTurn();
  towersTurn();
  
  if (checkLevelCompletion()) {
    handleLevelTransition();
  }
  
  player.addScore(1);
  player.addMana(1);
}

void Game::showSpellInfo() {
  std::cout << "\n=== SPELL INFO ===" << std::endl;
  std::cout << "Available Spells:" << std::endl;
  std::cout << "Fireball - 15 damage, range 3, 10 mana" << std::endl;
  std::cout << "  (Shoots through 1 cell in direction)" << std::endl;
  std::cout << "Explosion - 20 damage, 3x3 area, 15 mana" << std::endl;
  std::cout << "  (Damages all enemies in adjacent cells)" << std::endl;
  std::cout << "Trap - 5 damage, places trap, 5 mana" << std::endl;
  std::cout << "  (Enemies take damage when stepping on 't')" << std::endl;
  std::cout << "Invisibility - 3 turns, 20 mana" << std::endl;
  std::cout << "  (Enemies cannot detect or attack you)" << std::endl;
  std::cout << "Summon - summons 1 ally, 25 mana" << std::endl;
  std::cout << "  (Ally fights enemies autonomously)" << std::endl;
  std::cout << "Enhance - boosts next spell, 15 mana" << std::endl;
  std::cout << "  (Fireball: +range/damage, Explosion: +area/damage)" << std::endl;
  std::cout << "  (Trap: +damage, Invisibility: +duration, Summon: +allies)" << std::endl;
  std::cout << "Press 1-5 to cast spells from your hand" << std::endl;
  std::cout << "Use w/a/s/d to choose direction for spells" << std::endl;
  std::cout << "\n\n=== GAME INFO ===" << std::endl;
  std::cout << "You can save/load the game through Game Menu"<< std::endl;
  std::cout << "==================" << std::endl;
}