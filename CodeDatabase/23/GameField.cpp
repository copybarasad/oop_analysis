#include "GameField.h"
#include "Constants.h"
#include "EventSystem.h"
#include "GameEvent.h"
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <stdexcept>

GameField::GameField(int width, int height)
    : width(width), height(height), player(nullptr),
      rng(std::chrono::steady_clock::now().time_since_epoch().count()) {

  if (width < MIN_FIELD_SIZE || width > MAX_FIELD_SIZE ||
      height < MIN_FIELD_SIZE || height > MAX_FIELD_SIZE) {
    throw std::invalid_argument("Field dimensions must be between 10 and 25");
  }

  cells = new Cell *[height];
  for (int i = 0; i < height; i++) {
    cells[i] = new Cell[width];
  }

  generateTerrain();
  placeSlowingCells();

  int player_x, player_y;
  findValidPosition(player_x, player_y);
  player = new Player(player_x, player_y);

  computeReachableFromPlayer();

  placeEnemies();
  placeBuildings();
  placeTowers();
}

GameField::GameField(const GameField &other)
    : width(other.width), height(other.height), player(nullptr), rng(other.rng),
      reachable(other.reachable) {

  cells = new Cell *[height];
  for (int i = 0; i < height; i++) {
    cells[i] = new Cell[width];
    for (int j = 0; j < width; j++) {
      cells[i][j] = other.cells[i][j];
    }
  }

  if (other.player) {
    player = new Player(*other.player);
  }

  for (const auto &kv : other.enemies) {
    enemies[kv.first] = new Enemy(*kv.second);
  }
  for (const auto &kv : other.buildings) {
    buildings[kv.first] = new EnemyBuilding(*kv.second);
  }
  for (const auto &kv : other.towers) {
    towers[kv.first] = new EnemyTower(*kv.second);
  }
}

GameField::GameField(GameField &&other) noexcept
    : width(other.width), height(other.height), cells(other.cells),
      player(other.player), enemies(std::move(other.enemies)),
      buildings(std::move(other.buildings)), towers(std::move(other.towers)),
      rng(other.rng), reachable(std::move(other.reachable)) {

  other.cells = nullptr;
  other.player = nullptr;
  other.width = 0;
  other.height = 0;
}

GameField::~GameField() {
  if (cells) {
    for (int i = 0; i < height; i++) {
      delete[] cells[i];
    }
    delete[] cells;
  }

  delete player;

  for (auto &kv : enemies)
    delete kv.second;
  for (auto &kv : buildings)
    delete kv.second;
  for (auto &kv : towers)
    delete kv.second;
}

GameField &GameField::operator=(const GameField &other) {
  if (this != &other) {
    if (cells) {
      for (int i = 0; i < height; i++) {
        delete[] cells[i];
      }
      delete[] cells;
    }

    delete player;
    for (auto &kv : enemies)
      delete kv.second;
    for (auto &kv : buildings)
      delete kv.second;
    for (auto &kv : towers)
      delete kv.second;

    width = other.width;
    height = other.height;
    rng = other.rng;
    reachable = other.reachable;

    cells = new Cell *[height];
    for (int i = 0; i < height; i++) {
      cells[i] = new Cell[width];
      for (int j = 0; j < width; j++) {
        cells[i][j] = other.cells[i][j];
      }
    }

    if (other.player) {
      player = new Player(*other.player);
    } else {
      player = nullptr;
    }

    enemies.clear();
    for (const auto &kv : other.enemies)
      enemies[kv.first] = new Enemy(*kv.second);
    buildings.clear();
    for (const auto &kv : other.buildings)
      buildings[kv.first] = new EnemyBuilding(*kv.second);
    towers.clear();
    for (const auto &kv : other.towers)
      towers[kv.first] = new EnemyTower(*kv.second);
  }
  return *this;
}

GameField &GameField::operator=(GameField &&other) noexcept {
  if (this != &other) {
    if (cells) {
      for (int i = 0; i < height; i++) {
        delete[] cells[i];
      }
      delete[] cells;
    }

    delete player;
    for (auto &kv : enemies)
      delete kv.second;
    for (auto &kv : buildings)
      delete kv.second;
    for (auto &kv : towers)
      delete kv.second;

    width = other.width;
    height = other.height;
    cells = other.cells;
    player = other.player;
    enemies = std::move(other.enemies);
    buildings = std::move(other.buildings);
    towers = std::move(other.towers);
    rng = other.rng;
    reachable = std::move(other.reachable);

    other.cells = nullptr;
    other.player = nullptr;
    other.width = 0;
    other.height = 0;
  }
  return *this;
}

int GameField::getWidth() const { return width; }

int GameField::getHeight() const { return height; }

CellType GameField::getCellType(int x, int y) const {
  if (!isPositionValid(x, y)) {
    return CellType::IMPASSABLE;
  }
  return cells[y][x].getType();
}

Player *GameField::getPlayer() const { return player; }

const std::map<std::pair<int, int>, Enemy *> &GameField::getEnemies() const {
  return enemies;
}
const std::map<std::pair<int, int>, EnemyBuilding *> &
GameField::getBuildings() const {
  return buildings;
}
const std::map<std::pair<int, int>, EnemyTower *> &
GameField::getTowers() const {
  return towers;
}
void GameField::updateTowers() {
  if (!player)
    return;
  for (auto &kv : towers) {
    auto *t = kv.second;
    if (!t->isAlive())
      continue;
    t->tickAndAttack(*this, *player);
  }
}

bool GameField::isPositionValid(int x, int y) const {
  return x >= 0 && x < width && y >= 0 && y < height;
}

bool GameField::hasEnemyAt(int x, int y) const {
  return enemies.find({x, y}) != enemies.end();
}

bool GameField::hasBuildingAt(int x, int y) const {
  return buildings.find({x, y}) != buildings.end();
}

void GameField::findValidPosition(int &x, int &y, bool requireReachable) {
  std::uniform_int_distribution<int> x_dist(0, width - 1);
  std::uniform_int_distribution<int> y_dist(0, height - 1);
  const int maxAttempts = width * height * 4;
  int attempts = 0;

  while (attempts < maxAttempts) {
    x = x_dist(rng);
    y = y_dist(rng);
    attempts++;

    if (getCellType(x, y) == CellType::IMPASSABLE)
      continue;
    if (hasEnemyAt(x, y) || hasBuildingAt(x, y) || hasTowerAt(x, y))
      continue;
    if (requireReachable && !isReachable(x, y))
      continue;

    return;
  }

  if (requireReachable) {
    for (int yy = 0; yy < height; ++yy) {
      for (int xx = 0; xx < width; ++xx) {
        if (getCellType(xx, yy) == CellType::IMPASSABLE)
          continue;
        if (hasEnemyAt(xx, yy) || hasBuildingAt(xx, yy) || hasTowerAt(xx, yy))
          continue;
        if (!isReachable(xx, yy))
          continue;
        x = xx;
        y = yy;
        return;
      }
    }
    findValidPosition(x, y, false);
  }
}

void GameField::computeReachableFromPlayer() {
  reachable.assign(height, std::vector<bool>(width, false));
  if (!player)
    return;

  int sx = player->getX();
  int sy = player->getY();
  if (!isPositionValid(sx, sy))
    return;
  if (getCellType(sx, sy) == CellType::IMPASSABLE)
    return;

  std::queue<std::pair<int, int>> q;
  q.push({sx, sy});
  reachable[sy][sx] = true;

  const int dx[4] = {1, -1, 0, 0};
  const int dy[4] = {0, 0, 1, -1};

  while (!q.empty()) {
    auto [cx, cy] = q.front();
    q.pop();

    for (int i = 0; i < 4; ++i) {
      int nx = cx + dx[i];
      int ny = cy + dy[i];
      if (!isPositionValid(nx, ny))
        continue;
      if (reachable[ny][nx])
        continue;
      if (getCellType(nx, ny) == CellType::IMPASSABLE)
        continue;

      reachable[ny][nx] = true;
      q.push({nx, ny});
    }
  }
}

bool GameField::isReachable(int x, int y) const {
  if (!isPositionValid(x, y))
    return false;
  if ((int)reachable.size() != height ||
      (height > 0 && (int)reachable[0].size() != width)) {
    return false;
  }
  return reachable[y][x];
}

void GameField::generateTerrain() {
  std::uniform_int_distribution<int> dist(0, 99);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (dist(rng) < INIT_WALL_PROBABILITY) {
        cells[y][x].setType(CellType::IMPASSABLE);
      } else {
        cells[y][x].setType(CellType::NORMAL);
      }
    }
  }

  smoothTerrain();
}

void GameField::smoothTerrain() {
  Cell **new_cells = new Cell *[height];
  for (int i = 0; i < height; i++) {
    new_cells[i] = new Cell[width];
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int wall_neighbors = countNeighbors(x, y, CellType::IMPASSABLE);

      if (wall_neighbors >= SMOOTH_WALL_NEIGHBOR_HIGH) {
        new_cells[y][x].setType(CellType::IMPASSABLE);
      } else if (wall_neighbors <= SMOOTH_WALL_NEIGHBOR_LOW) {
        new_cells[y][x].setType(CellType::NORMAL);
      } else {
        new_cells[y][x].setType(cells[y][x].getType());
      }
    }
  }

  for (int i = 0; i < height; i++) {
    delete[] cells[i];
  }
  delete[] cells;
  cells = new_cells;
}

int GameField::countNeighbors(int x, int y, CellType type) {
  int count = 0;
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      if (dx == 0 && dy == 0)
        continue;

      int nx = x + dx;
      int ny = y + dy;

      if (isPositionValid(nx, ny) && cells[ny][nx].getType() == type) {
        count++;
      }
    }
  }
  return count;
}

void GameField::placeSlowingCells() {
  int passable_count = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (cells[y][x].isPassable()) {
        passable_count++;
      }
    }
  }

  int slowing_count = passable_count / SLOWING_CELLS_DIVISOR;
  std::uniform_int_distribution<int> dist(0, passable_count - 1);

  int placed = 0;
  while (placed < slowing_count) {
    int target = dist(rng);
    int current = 0;

    for (int y = 0; y < height && placed < slowing_count; y++) {
      for (int x = 0; x < width && placed < slowing_count; x++) {
        if (cells[y][x].isPassable() &&
            cells[y][x].getType() == CellType::NORMAL) {
          if (current == target) {
            cells[y][x].setType(CellType::SLOWING);
            placed++;
            break;
          }
          current++;
        }
      }
    }
  }
}

bool GameField::movePlayer(int dx, int dy) {
  if (!player)
    return false;

  if (player->getIsSlowed()) {
    return false;
  }

  int new_x = player->getX() + dx;
  int new_y = player->getY() + dy;

  if (!isPositionValid(new_x, new_y)) {
    return false;
  }

  CellType cell_type = getCellType(new_x, new_y);
  if (cell_type == CellType::IMPASSABLE) {
    return false;
  }

  if (auto it = enemies.find({new_x, new_y}); it != enemies.end()) {
    int damage = it->second->getDamage();
    player->takeDamage(damage);

    
    if (EventSystem::getInstance().isLoggingEnabled()) {
      DamageEvent event(it->second->getName(), "Player", damage);
      EventSystem::getInstance().notifyEvent(event);
    }
    return true;
  }

  if (buildings.find({new_x, new_y}) != buildings.end())
    return false;
  if (towers.find({new_x, new_y}) != towers.end())
    return false;

  int old_x = player->getX();
  int old_y = player->getY();
  player->setPosition(new_x, new_y);

  
  if (EventSystem::getInstance().isLoggingEnabled()) {
    MoveEvent event(old_x, old_y, new_x, new_y);
    EventSystem::getInstance().notifyEvent(event);
  }

  checkTrapAt(new_x, new_y);

  if (cell_type == CellType::SLOWING) {
    player->setSlowedNextTurn(true);
  }

  return true;
}

bool GameField::attackEnemy(int dx, int dy) {
  if (!player)
    return false;

  AttackType attack_type = player->getAttackType();
  int max_range = (attack_type == AttackType::RANGE)
                      ? RANGE_ATTACK_MAX_DISTANCE
                      : MELEE_ATTACK_MAX_DISTANCE;

  for (int distance = 1; distance <= max_range; distance++) {
    int target_x = player->getX() + dx * distance;
    int target_y = player->getY() + dy * distance;

    if (!isPositionValid(target_x, target_y)) {
      break;
    }

    if (getCellType(target_x, target_y) == CellType::IMPASSABLE) {
      break;
    }

    auto enemy_it = enemies.find({target_x, target_y});
    if (enemy_it != enemies.end()) {
      int damage = player->getCurrentDamage();
      std::string enemyName = enemy_it->second->getName();
      enemy_it->second->takeDamage(damage);
      player->addScore(SCORE_PER_ENEMY);

      
      if (EventSystem::getInstance().isLoggingEnabled()) {
        DamageEvent event("Player", enemyName, damage);
        EventSystem::getInstance().notifyEvent(event);
      }
      return true;
    }

    auto building_it = buildings.find({target_x, target_y});
    if (building_it != buildings.end()) {
      int damage = player->getCurrentDamage();
      building_it->second->takeDamage(damage);
      player->addScore(SCORE_PER_ENEMY);

      
      if (EventSystem::getInstance().isLoggingEnabled()) {
        DamageEvent event("Player", "Building", damage);
        EventSystem::getInstance().notifyEvent(event);
      }
      return true;
    }

    auto tower_it = towers.find({target_x, target_y});
    if (tower_it != towers.end()) {
      int damage = player->getCurrentDamage();
      tower_it->second->takeDamage(damage);
      player->addScore(SCORE_PER_ENEMY);

      
      if (EventSystem::getInstance().isLoggingEnabled()) {
        DamageEvent event("Player", "Tower", damage);
        EventSystem::getInstance().notifyEvent(event);
      }
      return true;
    }
  }

  return false;
}

void GameField::moveEnemies() {
  if (!player)
    return;

  std::vector<std::pair<std::pair<int, int>, Enemy *>> toUpdate;
  for (auto &kv : enemies) {
    Enemy *enemy = kv.second;
    if (enemy->isAlive()) {
      int dx = abs(player->getX() - enemy->getX());
      int dy = abs(player->getY() - enemy->getY());
      int distance = dx + dy;
      if (distance == 1) {
        enemy->attackPlayer(player);
      } else {
        int ox = enemy->getX(), oy = enemy->getY();
        enemy->moveTowardsPlayer(player->getX(), player->getY(), *this);
        if (enemy->getX() != ox || enemy->getY() != oy) {
          toUpdate.push_back({{ox, oy}, enemy});
        }
      }
    }
  }
  for (auto &upd : toUpdate) {
    enemies.erase(upd.first);
    enemies[{upd.second->getX(), upd.second->getY()}] = upd.second;
    checkTrapAt(upd.second->getX(), upd.second->getY());
  }
}

void GameField::updateBuildings() {
  for (auto &kv : buildings) {
    EnemyBuilding *building = kv.second;
    if (!building->isAlive())
      continue;
    if (building->tick()) {
      Enemy *new_enemy = building->spawnEnemy();
      if (new_enemy) {
        int spawn_x, spawn_y;
        bool found = false;

        for (int dy = -1; dy <= 1 && !found; dy++) {
          for (int dx = -1; dx <= 1 && !found; dx++) {
            if (dx == 0 && dy == 0)
              continue;

            spawn_x = building->getX() + dx;
            spawn_y = building->getY() + dy;

            if (isPositionValid(spawn_x, spawn_y) &&
                getCellType(spawn_x, spawn_y) != CellType::IMPASSABLE &&
                !hasEnemyAt(spawn_x, spawn_y) &&
                !hasBuildingAt(spawn_x, spawn_y) &&
                !hasTowerAt(spawn_x, spawn_y) &&
                isReachable(spawn_x, spawn_y) &&
                !(player && player->getX() == spawn_x &&
                  player->getY() == spawn_y)) {

              new_enemy->setPosition(spawn_x, spawn_y);
              enemies[{spawn_x, spawn_y}] = new_enemy;
              found = true;
            }
          }
        }

        if (!found) {
          delete new_enemy;
        }
      }
    }
  }
}

void GameField::display() const {
  std::cout << "\033[2J\033[H";
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      char symbol = '.';

      if (player && player->getX() == x && player->getY() == y) {
        symbol = 'P';
      } else if (hasEnemyAt(x, y)) {
        symbol = 'E';
      } else if (hasBuildingAt(x, y)) {
        symbol = 'B';
      } else if (hasTowerAt(x, y)) {
        symbol = 'T';
      } else if (hasTrapAt(x, y)) {
        symbol = '*';
      } else {
        switch (getCellType(x, y)) {
        case CellType::IMPASSABLE:
          symbol = '#';
          break;
        case CellType::SLOWING:
          symbol = '~';
          break;
        case CellType::NORMAL:
          symbol = '.';
          break;
        }
      }

      std::cout << symbol << ' ';
    }
    std::cout << std::endl;
  }

  if (player) {
    std::cout << "\nPlayer Stats:" << std::endl;
    std::cout << "HP: " << player->getHp() << std::endl;
    std::cout << "Damage: " << player->getCurrentDamage() << std::endl;
    std::cout << "Attack Type: "
              << (player->getAttackType() == AttackType::RANGE ? "RANGE"
                                                               : "MELEE")
              << std::endl;
    std::cout << "Score: " << player->getScore() << std::endl;
    std::cout << "Position: (" << player->getX() << ", " << player->getY()
              << ")" << std::endl;
    if (player->getIsSlowed()) {
      std::cout << "Status: SLOWED (cannot move next turn)" << std::endl;
    }
    if (player->getHand()) {
      const auto &spells = player->getHand()->getSpells();
      std::cout << "Spells in hand:" << std::endl;
      for (int i = 0; i < (int)spells.size(); ++i) {
        std::cout << i << ") " << spells[i]->name() << std::endl;
      }
    }
  }

  std::cout << "Enemies: " << enemies.size() << std::endl;
  std::cout << "Buildings: " << buildings.size() << std::endl;
  std::cout << "Towers: " << towers.size() << std::endl;
}

void GameField::removeDeadEnemies() {
  for (auto it = enemies.begin(); it != enemies.end();) {
    if (!it->second->isAlive()) {
      
      if (EventSystem::getInstance().isLoggingEnabled()) {
        EnemyDeathEvent event(it->second->getName(), it->second->getX(),
                              it->second->getY());
        EventSystem::getInstance().notifyEvent(event);
      }
      delete it->second;
      it = enemies.erase(it);
    } else
      ++it;
  }
  for (auto it = buildings.begin(); it != buildings.end();) {
    if (!it->second->isAlive()) {
      
      if (EventSystem::getInstance().isLoggingEnabled()) {
        EnemyDeathEvent event("Building", it->second->getX(),
                              it->second->getY());
        EventSystem::getInstance().notifyEvent(event);
      }
      delete it->second;
      it = buildings.erase(it);
    } else
      ++it;
  }
  for (auto it = towers.begin(); it != towers.end();) {
    if (!it->second->isAlive()) {
      
      if (EventSystem::getInstance().isLoggingEnabled()) {
        EnemyDeathEvent event("Tower", it->second->getX(), it->second->getY());
        EventSystem::getInstance().notifyEvent(event);
      }
      delete it->second;
      it = towers.erase(it);
    } else
      ++it;
  }
}

void GameField::addEnemy(Enemy *enemy) {
  enemies[{enemy->getX(), enemy->getY()}] = enemy;
}

void GameField::addBuilding(EnemyBuilding *building) {
  buildings[{building->getX(), building->getY()}] = building;
}

void GameField::placeEnemies() {
  int total_cells = width * height;
  int enemy_count =
      std::max(ENEMY_MIN_COUNT, total_cells / ENEMY_DENSITY_DIVISOR);

  std::uniform_int_distribution<int> enemy_type_dist(0, 2);

  for (int i = 0; i < enemy_count; i++) {
    int x, y;
    findValidPosition(x, y, true);

    while (player && (abs(x - player->getX()) + abs(y - player->getY()) < 3)) {
      findValidPosition(x, y, true);
    }

    int enemy_type = enemy_type_dist(rng);
    Enemy *enemy = nullptr;

    switch (enemy_type) {
    case 0:
      enemy = new Enemy(x, y, "Goblin", ENEMY0_HP, ENEMY0_DMG);
      break;
    case 1:
      enemy = new Enemy(x, y, "Orc", ENEMY1_HP, ENEMY1_DMG);
      break;
    case 2:
      enemy = new Enemy(x, y, "Troll", ENEMY2_HP, ENEMY2_DMG);
      break;
    }

    if (enemy) {
      enemies[{x, y}] = enemy;
    }
  }
}

void GameField::placeBuildings() {
  int total_cells = width * height;
  int building_count =
      std::max(BUILDING_MIN_COUNT, total_cells / BUILDING_DENSITY_DIVISOR);

  std::uniform_int_distribution<int> building_type_dist(0, 2);
  std::uniform_int_distribution<int> cooldown_dist(BUILD_COOLDOWN_MIN,
                                                   BUILD_COOLDOWN_MAX);

  for (int i = 0; i < building_count; i++) {
    int x, y;
    findValidPosition(x, y, true);

    while (player && (abs(x - player->getX()) + abs(y - player->getY()) < 4)) {
      findValidPosition(x, y, true);
    }

    int building_type = building_type_dist(rng);
    int cooldown = cooldown_dist(rng);
    EnemyBuilding *building = nullptr;

    switch (building_type) {
    case 0:
      building = new EnemyBuilding(x, y, "Skeleton", BUILD0_ENEMY_HP,
                                   BUILD0_ENEMY_DMG, cooldown, BUILD0_HP);
      break;
    case 1:
      building = new EnemyBuilding(x, y, "Zombie", BUILD1_ENEMY_HP,
                                   BUILD1_ENEMY_DMG, cooldown, BUILD1_HP);
      break;
    case 2:
      building = new EnemyBuilding(x, y, "Demon", BUILD2_ENEMY_HP,
                                   BUILD2_ENEMY_DMG, cooldown, BUILD2_HP);
      break;
    }

    if (building) {
      buildings[{x, y}] = building;
    }
  }
}

void GameField::placeTowers() {
  int total_cells = width * height;
  int count = std::max(0, total_cells / 200);
  for (int i = 0; i < count; ++i) {
    int x, y;
    findValidPosition(x, y, true);
    while (player && (abs(x - player->getX()) + abs(y - player->getY()) < 5)) {
      findValidPosition(x, y, true);
    }
    auto *t = new EnemyTower(x, y, TOWER_HP, TOWER_ATTACK_RANGE,
                             DIRECT_SPELL_WEAK_DAMAGE, TOWER_COOLDOWN);
    towers[{x, y}] = t;
  }
}

void GameField::placeTrap(int x, int y, int damage) { traps[{x, y}] = damage; }

void GameField::checkTrapAt(int x, int y) {
  auto it = traps.find({x, y});
  if (it == traps.end())
    return;
  int trapDamage = it->second;

  auto eIt = enemies.find({x, y});
  if (eIt != enemies.end()) {
    eIt->second->takeDamage(trapDamage);

    
    if (EventSystem::getInstance().isLoggingEnabled()) {
      DamageEvent event("Trap", eIt->second->getName(), trapDamage);
      EventSystem::getInstance().notifyEvent(event);
    }
  }
  traps.erase(it);
}

bool GameField::hasTrapAt(int x, int y) const {
  return traps.find({x, y}) != traps.end();
}

void GameField::addTower(EnemyTower *tower) {
  towers[{tower->getX(), tower->getY()}] = tower;
}
bool GameField::hasTowerAt(int x, int y) const {
  return towers.find({x, y}) != towers.end();
}
