#include "game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <cmath>
#include <limits>
#include <sstream>
#include <fstream>
#include <filesystem>

GameException::GameException(const std::string& message): message(message) {}
const char* GameException::what() const noexcept { return message.c_str(); }
InvalidCommandException::InvalidCommandException(const std::string& details): GameException("Invalid command: " + details) {}
LevelLoadException::LevelLoadException(const std::string& details): GameException("Failed to load level: " + details) {}
GameLogicException::GameLogicException(const std::string& details): GameException("Game logic error" + details) {}

const std::vector<Enemy>& Game::get_enemies() const { return enemies; }
const std::vector<EnemyBuilding>& Game::get_buildings() const { return buildings; }
const std::vector<EnemyTower>& Game::get_towers() const { return towers; }
const std::vector<Ally>& Game::get_allies() const { return allies; }
const Field& Game::get_field() const { return field; }
const Player& Game::get_player() const { return player; }
Player& Game::get_player() { return player; }

Game::Game(int width, int height, int numEnemies): state(GameState::Starting), field(width, height), 
                                                   player(0, 0, 100, 25, 15, 40, 5, static_cast<size_t>(5)), allies() {
  srand(time(0));
  try {
    loadLevel();
    initialGrid = field.getGrid();
    initializeEnemies(numEnemies);
    initializeBuildings(2);
    initializeTowers(1);
    updateField();
  } catch (const LevelLoadException& e) {
    std::cerr << "Error during initialization: " << e.what() << std::endl;
    state = GameState::Exiting;
  }
}

void Game::run() {
  while (state != GameState::Exiting) {
    try {
      std::string input;
      switch(state) {
        case GameState::Starting: startGame(); break;
        case GameState::LevelRunning:
          alliesTurn();
          enemiesTurn();
          buildingsAndTowersTurn();
          updateField();
          displayGameState();

          std::cout << "Enter command (move <direction: w/a/s/d>, cast <number> <direction: w/a/s/d>, mode, buy, save, quit): ";
          std::getline(std::cin, input);
          if (input.empty()) continue;
          processCommand(input);

          if (isLevelWon()) {
            state = GameState::LevelWon;
            handleGameOver();
          } else if (isLevelLost()) {
            state = GameState::LevelLost;
            handleGameOver();
          }
          break;
        case GameState::LevelWon:
        case GameState::LevelLost:
          if (!restartGame()) {
            state = GameState::Exiting;
          } else {
            state = GameState::Starting;
          }
          break;
        default:
          throw GameLogicException("Invalid game state encountered.");
      }
    } catch (const GameException& e) {
      std::cerr << "Game error: " << e.what() << std::endl;
      if (!restartGame()) {
        state = GameState::Exiting;
      }
    }
  }
}

void Game::processCommand(const std::string& input) {
  if (state != GameState::LevelRunning) {
    throw InvalidCommandException("Commands can only be processed during level running.");
  }
  try {
    if (input == "quit") {
      state = GameState::Exiting;
      return;
    }
    playerTurn(input);
  } catch (const std::invalid_argument&) {
      throw InvalidCommandException("Invalid argument format.");
  }
}

bool Game::isLevelWon() const {
  bool enemiesDead = std::all_of(enemies.begin(), enemies.end(), [](const Enemy& e) {return !e.isAlive();});
  bool towersDead = std::all_of(towers.begin(), towers.end(), [](const EnemyTower& t) {return !t.isAlive();});
  bool buildingsDead = std::all_of(buildings.begin(), buildings.end(), [](const EnemyBuilding& b) {return !b.isAlive();});
  return enemiesDead && towersDead && buildingsDead;
}

bool Game::isLevelLost() const {
  return player.getHealth() <= 0;
}

void Game::startGame() {
  std::cout << "Welcome to the game.\n" << std::endl;
  state = GameState::LevelRunning;
}

void Game::loadLevel() {
  enemies.clear();
  buildings.clear();
  towers.clear();
  traps.clear();
  allies.clear();
}

void Game::playerTurn(const std::string& command) {
  if (player.isSlowed()) {
    std::cout << "You are slowed and cannot move this turn.\n";
    player.setSlowed(false);
    return;
  }

  std::istringstream iss(command);
  std::string action;
  iss >> action;

  if (action == "mode") {
    player.switchMode();
    return;
  }
  if (action == "buy") {
    player.buySpell();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return;
  }
  if (action == "save") {
    std::string filename;
    std::cout << "Enter save file name: ";
    std::cin >> filename;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    saveGame(filename);
    return;
  }
  if (action == "cast") {
    int spellIndex;
    char direction;
    iss >> spellIndex >> direction;
    if (iss.fail() || (direction != 'w' && direction != 'a' && direction != 's' && direction != 'd')) {
      throw InvalidCommandException("Invalid cast command format. Use 'cast index direction', where direction w/a/s/d.\n");
    }

    int dx = 0, dy = 0;
    switch (direction) {
      case 'w': dy -= 1; break;
      case 's': dy = 1; break;
      case 'a': dx -= 1; break;
      case 'd': dx = 1; break;
      default: return;
    }
    castSpell(spellIndex, dx, dy);
    return;
  }
  if (action == "move") {
    char direction;
    iss >> direction;
    if (iss.fail() || (direction != 'w' && direction != 'a' && direction != 's' && direction != 'd')) {
      throw InvalidCommandException("Invalid move command format. Use 'move direction', where direction w/a/s/d.\n");
    }    
    int dx = 0, dy = 0;
    switch (direction) {
    case 'w': dy--; break;
    case 's': dy++; break;
    case 'a': dx--; break;
    case 'd': dx++; break;
    default: return;
    }  
    if (player.getMode() == Player::Ranged) {
      if (player.getAmmo()<=0) {
        std::cout << "Out of ammo! Switch to Melee or kill enemy to reload.\n";
        return;
      }
      shootRanged(dx, dy);
    } else {
      int newX = player.getX() + dx;
      int newY = player.getY() + dy;

      if (newX>=0 && newX<field.getWidth() && newY>=0 && newY<field.getHeight()) {
        Cell::State cellState = field.getCell(newX, newY).getState();
        if (cellState == Cell::Enemy) {
          for (auto& enemy: enemies) {
            if (enemy.getX() == newX && enemy.getY() == newY && enemy.isAlive()) {
              enemy.takeDamage(player.getDamage());
              if (!enemy.isAlive()) {
                player.addScore(10);
              }
              std::cout << "Attacked enemy in melee.\n";
              break;
            }
          }
        } else if (cellState == Cell::EnemyBuilding) {
          applyDamage(newX, newY, player.getDamage());
        } else if (cellState == Cell::EnemyTower) {
          applyDamage(newX, newY, player.getDamage());
        } else if (cellState == Cell::Empty || cellState == Cell::SlowCell || cellState == Cell::Trap) {
          player.move(newX, newY);
          if (field.getCell(newX, newY).getState() == Cell::Trap) {
            removeTrap(newX, newY);
            std::cout << "You disarmed a trap.\n";
          }
          if (field.getCell(newX, newY).getState() == Cell::SlowCell) {
            player.setSlowed(true);
          }
        }
      }
    }
    return;
  }

  throw InvalidCommandException("Unknown command.\n");
}

void Game::alliesTurn() {
  for (auto& ally: allies) {
    if (!ally.isAlive()) continue;
    if (ally.isSlowed()) {
      ally.setSlowed(false);
      continue;
    }

    int minDistance = INT16_MAX;
    int targetX = -1, targetY = -1;
    for (const auto& enemy: enemies) {
      if (enemy.isAlive()) {
        int distance = std::abs(enemy.getX() - ally.getX()) + std::abs(enemy.getY() - ally.getY());
        if (distance<minDistance) {
          minDistance = distance;
          targetX = enemy.getX();
          targetY = enemy.getY();
        }
      }
    }

    for (const auto& building: buildings) {
      int distance = std::abs(building.getX() - ally.getX()) + std::abs(building.getY() - ally.getY());
      if (distance<minDistance) {
        minDistance = distance;
        targetX = building.getX();
        targetY = building.getY();
      }
    }

    for (const auto& tower: towers) {
      int distance = std::abs(tower.getX() - ally.getX()) + std::abs(tower.getY() - ally.getY());
      if (distance<minDistance) {
        minDistance = distance;
        targetX = tower.getX();
        targetY = tower.getY();
      }
    }

    if (targetX != -1) {
      if (minDistance == 1) {
        applyDamage(targetX, targetY, ally.getDamage());
      } else {
        int newX = ally.getX() + (targetX > ally.getX() ? 1 : (targetX < ally.getX() ? -1 : 0));
        int newY = ally.getY() + (targetY > ally.getY() ? 1 : (targetY < ally.getY() ? -1 : 0));

        if (newX>=0 && newX<field.getWidth() && newY>=0 && newY<field.getHeight()) {
          Cell::State cellState = field.getCell(newX, newY).getState();
          if (cellState == Cell::Empty || cellState == Cell::SlowCell || cellState == Cell::Trap) {
            ally.move(newX, newY);
            if (cellState == Cell::Trap) {
              removeTrap(newX, newY);
            }
            if (cellState == Cell::SlowCell) {
              ally.setSlowed(true);
            }
          }
        }
      }
    }
  }

  allies.erase(std::remove_if(allies.begin(), allies.end(), [](const Ally& ally) {
    return !ally.isAlive();
  }), allies.end());
}

void Game::enemiesTurn() {
  for (auto& enemy: enemies) {
    if (!enemy.isAlive()) continue;

    if (std::abs(player.getX()-enemy.getX()) + std::abs(player.getY()-enemy.getY()) == 1) {
      player.takeDamage(enemy.getDamage());
      std::cout << "Enemy attacked you.\n";
    } else {
      auto [newX, newY] = enemy.decideMove(field.getWidth(), field.getHeight(), field, player.getX(), player.getY(), allies);
      Cell::State cellState = field.getCell(newX, newY).getState();
      if (cellState == Cell::Empty || cellState == Cell::SlowCell ||  cellState == Cell::Trap) {
        enemy.move(newX, newY);
        if (cellState == Cell::Trap) {
          auto it = std::find_if(traps.begin(), traps.end(), [newX, newY](const auto& trap) {
            auto [tx, ty, _] = trap;
            return tx == newX && ty == newY;
          });
          if (it != traps.end()) {
            auto [_, __, damage] = *it;
            enemy.takeDamage(damage);
            std::cout << "Enemy stepped on a trap and took " << damage << " damage.\n";
            removeTrap(newX, newY);
          }
        }
      }
    }
  }

  for (auto& enemy: enemies) {
    if (!enemy.isAlive()) continue;
    for (auto& ally: allies) {
      if (ally.isAlive() && std::abs(ally.getX() - enemy.getX()) + std::abs(ally.getY() - enemy.getY()) == 1) {
        ally.takeDamage(enemy.getDamage());
      }
    }
  }

  for (auto& building: buildings) {
    if (building.isAlive() && building.update()) {
      spawnByBuildings(building.getX(), building.getY());
    }
  }
}

void Game::buildingsAndTowersTurn() {
  for (auto& tower: towers) {
    if (tower.isAlive() && tower.update(player.getX(), player.getY())) {
      DirectDamageSpell spell(15);
      spell.apply(*this, player.getX(), player.getY(), tower.getX(), tower.getY());
      std::cout << "Enemy tower cast weakened direct damage on you.\n";
    }
  }
}

void Game::displayGameState() const {
  field.display();
  std::cout << "Health: " << player.getHealth()
            << ", Score: " << player.getScore()
            << ", Mode: " << (player.getMode() == Player::Melee ? "Melee" : "Ranged")
            << ", Ammo: " << player.getAmmo()
            <<", Spells: " << player.getSpellHand().getSize() 
            << "/" << player.getSpellHand().getMaxSize() << std::endl;
}

void Game::handleGameOver() {
  if (state == GameState::LevelWon) {
    std::cout << "You won the level." << std::endl;
    currentLevel++;
  } else if (state == GameState::LevelLost) {
    std::cout << "You lost the level." << std::endl;
  }
}

bool Game::restartGame() {
  std::cout << "restart game? (y/n): ";
  char choice;
  std::cin >> choice;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return choice == 'y' || choice == 'Y';
}

void Game::initializeEnemies(int numEnemies) {
  for (int i=0; i<numEnemies; ++i) {
    int x = rand() % field.getWidth();
    int y = rand() % field.getHeight();
    while (field.getCell(x, y).getState() != Cell::Empty || (x == player.getX() && y == player.getY())) {
      x = rand() % field.getWidth();
      y = rand() % field.getHeight();
    }
    enemies.emplace_back(x, y, 50, 15);
  }
}

void Game::initializeBuildings(int numBuildings) {
  for (int i=0; i<numBuildings; i++) {
    int x = rand() % field.getWidth();
    int y = rand() % field.getHeight();
    while (field.getCell(x, y).getState() != Cell::Empty || (x == player.getX() && y == player.getY())) {
      x = rand() % field.getWidth();
      y = rand() % field.getHeight();
    }
    buildings.emplace_back(x, y, 10, 100);
  }
}

void Game::initializeTowers(int numTowers) {
  for (int i=0; i<numTowers; i++) {
    int x = rand() % field.getWidth();
    int y = rand() % field.getHeight();
    while (field.getCell(x, y).getState() != Cell::Empty || (x == player.getX() && y == player.getY())) {
      x = rand() % field.getWidth();
      y = rand() % field.getHeight();
    }
    towers.emplace_back(x, y, 100, 3);
  }
}

void Game::updateField() {
  field.resetGrid(initialGrid);

  for (const auto& trap: traps) {
    auto [x, y, _] = trap;
    field.setCell(x, y, Cell::Trap);
  }

  field.setCell(player.getX(), player.getY(), Cell::Player);

  for (const auto& enemy: enemies) {
    if (enemy.isAlive()) field.setCell(enemy.getX(), enemy.getY(), Cell::Enemy);
  }

  for (const auto& building: buildings) {
    if (building.isAlive()) field.setCell(building.getX(), building.getY(), Cell::EnemyBuilding);
  }

  for (const auto& tower: towers) {
    if (tower.isAlive()) field.setCell(tower.getX(), tower.getY(), Cell::EnemyTower);
  }

  for (const auto& ally: allies) {
    if (ally.isAlive()) field.setCell(ally.getX(), ally.getY(), Cell::Ally);
  }
}

void Game::shootRanged(int dx, int dy) {
  int px = player.getX();
  int py = player.getY();
  bool hit = false;
  bool blocked = false;
  player.useAmmo(1);

  for (int distance=1; distance<=3; ++distance) {
    int tx = px+distance*dx;
    int ty = py+distance*dy;
    if (tx<0 || tx>=field.getWidth() || ty<0 || ty>=field.getHeight()) {
      break;
    }
    Cell::State state = field.getCell(tx, ty).getState();
    if (state == Cell::Wall) {
      blocked = true;
      break;
    }
    if (state == Cell::Enemy) {
      for (auto& enemy: enemies) {
        if (enemy.getX() == tx && enemy.getY() == ty && enemy.isAlive()) {
          enemy.takeDamage(player.getDamage());
          if (!enemy.isAlive()) {
            player.addScore(10);
            player.reloadAmmo(3);
          }
          std::cout << "Shot hit enemy.\n";
           hit = true;
           return;
        }
      }
    } else if (state==Cell::EnemyBuilding || state==Cell::EnemyTower) {
      applyDamage(tx, ty, player.getDamage());
      hit = true;
      return;
    }
  }
  if (blocked) {
    std::cout << "Shot blocked by wall.\n";
  } else if (!hit) {
    std::cout << "Missed! No enemy or building/tower at range.\n";
  }
}

void Game::castSpell(int spellIndex, int dx, int dy) {
  int px = player.getX();
  int py = player.getY();
  auto [tx, ty] = player.getSpellHand().selectSpell(spellIndex).findTarget(*this, px, py, dx, dy);
  if (tx!=-1) {
    player.castSpell(spellIndex, tx, ty, *this);
    std::cout << "Spell cast.\n";
  } else {
    std::cout << "No enemy in range for spell.\n";
  }
}

void Game::applyDamage(int x, int y, int damage) {
  if (x == player.getX() && y == player.getY()) {
    player.takeDamage(damage);
    return;
  }

  for (auto& enemy: enemies) {
    if (enemy.getX()==x && enemy.getY()==y && enemy.isAlive()) {
      enemy.takeDamage(damage);
      if (!enemy.isAlive()) {
        player.addScore(10);
      }
      break;
    }
  }
  for (auto& building: buildings) {
    if (building.getX() == x && building.getY() == y && building.isAlive()) {
      building.takeDamage(damage);
      if (!building.isAlive()) {
        std::cout << "You destroyed an enemy building!\n";
        player.reloadAmmo(5);
        player.addScore(20);  
      } else {
        std::cout << "You damaged an enemy building.\n";
      }
      return;
    }
  }
  for (auto& tower: towers) {
    if (tower.getX() == x && tower.getY() == y && tower.isAlive()) {
      tower.takeDamage(damage);
      if (!tower.isAlive()) {
        std::cout << "You destroyed an enemy tower!\n";
        player.addScore(30);
      } else {
        std::cout << "You damaged an enemy tower.\n";
      }
      return;
    }
  }
  for (auto& ally: allies) {
    if (ally.getX() == x && ally.getY() == y) {
      ally.takeDamage(damage);
    }
  }
}

void Game::spawnByBuildings(int bx, int by) {
  std::vector<std::pair<int, int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
  std::shuffle(directions.begin(), directions.end(), std::default_random_engine(std::random_device{}()));

  for (auto& dir: directions) {
    int nx = bx+dir.first;
    int ny = by+dir.second;
    if (nx>=0 && nx<field.getWidth() && ny >= 0 && ny < field.getHeight() 
       && field.getCell(nx, ny).getState() == Cell::Empty 
       && !(nx == player.getX() && ny == player.getY())) {
        enemies.emplace_back(nx, ny, 50, 15);
        return;
       }
  }
}

void Game::placeTrap(int x, int y, int damage) {
  if (x >= 0 && x < field.getWidth() && y >= 0 && y < field.getHeight() &&
      field.getCell(x, y).getState() == Cell::Empty) {
    field.setCell(x, y, Cell::Trap);
    traps.emplace_back(x, y, damage);
  }
}

void Game::removeTrap(int x, int y) {
  traps.erase(std::remove_if(traps.begin(), traps.end(), [x, y](const auto& trap) {
    auto [tx, ty, _] = trap;
    return tx == x && ty == y;
  }), traps.end());
}

void Game::addAlly(int x, int y) {
  allies.emplace_back(x, y, 50, 15);
}

void Game::saveGame(const std::string& filename) const {
  std::ofstream ofs(filename);
  if (!ofs) throw GameException("Failed to open save file for writing.");

  ofs << static_cast<int>(state) << '\n';
  ofs << currentLevel << '\n';

  ofs << player.getX() << ' ' << player.getY() << ' ' << player.getHealth() 
       << ' ' << player.getScore() << ' ' << player.getAmmo() << ' '
       << static_cast<int>(player.getMode()) << '\n';

  ofs << player.getSpellHand().getSize() << '\n';
  for (const auto& spellPtr: player.getSpellHand().getSpells()) {
    SpellType type = spellPtr->getType();
    ofs << static_cast<int>(type) << '\n';
  }

  ofs << player.getEnhancementLevel() << '\n';

  ofs << enemies.size() << "\n";
  for (const auto& enemy: enemies) {
    ofs << enemy.getX() << " " << enemy.getY() << " " << enemy.getHealth() << ' ' << enemy.getDamage() << "\n";
  }

  ofs << buildings.size() << "\n";
  for (const auto& building: buildings) {
    ofs << building.getX() << " " << building.getY() << " " << building.getHealth() << ' ' << building.getCurrentTimer() << "\n";
  }

  ofs << towers.size() << "\n";
  for (const auto& tower: towers) {
    ofs << tower.getX() << " " << tower.getY() << " " << tower.getHealth() << ' ' << tower.getCurrentCooldown() << "\n";
  }

  ofs << allies.size() << "\n";
  for (const auto& ally: allies) {
    ofs << ally.getX() << " " << ally.getY() << " " << ally.getHealth() << ' ' << ally.getDamage() << "\n";
  }

  ofs << traps.size() << "\n";
  for (const auto& trap: traps) {
    auto [x, y, damage] = trap;
    ofs << x << " " << y << " " << damage << "\n";
  }

  ofs << field.getWidth() << ' ' << field.getHeight() << '\n';
  for (int y = 0; y < field.getHeight(); ++y) {
    for (int x = 0; x <field.getWidth(); ++x) {
      ofs << static_cast<int>(initialGrid[y][x].getState()) << ' ';
    }
    ofs << '\n';
  } 
}

bool Game::loadGame(const std::string& filename) {
  std::ifstream ifs(filename);
  if (!ifs) GameException("Failed to open save file for reading.\n");

  int stateInt;
  ifs >> stateInt;
  state = static_cast<GameState>(stateInt);
  ifs >> currentLevel;

  int playerX, playerY, playerHealth, playerScore, playerAmmo, modeInt;
  ifs >> playerX >> playerY >> playerHealth >> playerScore >> playerAmmo >> modeInt;
  player.setPosition(playerX, playerY);
  player.setHealth(playerHealth);
  player.setScore(playerScore);
  player.setAmmo(playerAmmo);
  player.setMode(static_cast<Player::Mode>(modeInt));
  
  size_t spellCount;
  ifs >> spellCount;
  player.getSpellHand().clear();
  for (size_t i = 0; i < spellCount; ++i) {
    int typeInt;
    ifs >> typeInt;
    SpellType type = static_cast<SpellType>(typeInt);
    auto spell = player.getSpellHand().createSpell(type);
    if (spell) {
      player.getSpellHand().addSpell(std::move(spell));
    }
  }

  int enhLevel;
  ifs >> enhLevel;

  size_t enemyCount;
  ifs >> enemyCount;
  enemies.clear();
  for (size_t i = 0; i < enemyCount; ++i) {
    int x, y, health, damage;
    ifs >> x >> y >> health >> damage;
    enemies.emplace_back(x, y, health, damage);
  }

  size_t buildingCount;
  ifs >> buildingCount;
  buildings.clear();
  for (size_t i = 0; i < buildingCount; ++i) {
    int x, y, health, currentTimer;
    ifs >> x >> y >> health >> currentTimer;
    buildings.emplace_back(x, y, health, currentTimer);
  }

  size_t towerCount;
  ifs >> towerCount;
  towers.clear();
  for (size_t i = 0; i < towerCount; ++i) {
    int x, y, health, currentCooldown;
    ifs >> x >> y >> health >> currentCooldown;
    towers.emplace_back(x, y, health, currentCooldown);
  }

  size_t allyCount;
  ifs >> allyCount;
  allies.clear();
  for (size_t i = 0; i < allyCount; ++i) {
    int x, y, health, damage;
    ifs >> x >> y >> health >> damage;
    allies.emplace_back(x, y, health, damage);
  }

  size_t trapCount;
  ifs >> trapCount;
  traps.clear();
  for (size_t i = 0; i < trapCount; ++i) {
    int x, y, damage;
    ifs >> x >> y >> damage;
    traps.emplace_back(x, y, damage);
  }

  int width, height;
  ifs >> height >> width;
  initialGrid.resize(height, std::vector<Cell>(width));
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int stateInt;
      ifs >> stateInt;
      initialGrid[y][x].setState(static_cast<Cell::State>(stateInt));
    }
  }

  updateField();
  return true;
}

std::pair<bool, std::string> Game::displayMainMenu() {
  std::cout << "1. New game\n2. Load game\nChoose an option: ";
  int choice;
  std::cin >> choice;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (choice == 1) {
    return {true, ""};
  } else if (choice == 2) {
    while (true) {
      std::string filename;
      std::cout << "Enter save file name: ";
      std::getline(std::cin, filename);
      if (std::filesystem::exists(filename)){
        return {false, filename};
      } else {
        std::cout << "File does not exist. Please enter a valid file name or choose '1' for new game.\n";
      }
    }
  } else {
    std::cout << "Invalid choice. Starting new game.\n";
    return {true, ""};
  }
}