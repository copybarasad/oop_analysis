#include "GameWorld.h"

GameWorld::GameWorld(int width, int height)
    : map(width, height), player(100, 10, {-1, -1}) {}

void GameWorld::InitEntities(int numEnemies, int numSpawners, int numTowers) {
  for (int i = 0; i < numEnemies; ++i) {
    auto posOpt = map.FindRandomFreeCell();
    if (!posOpt.has_value()) break;
    enemies.push_back(std::make_unique<Enemy>(20, 5, posOpt.value()));
  }

  for (int i = 0; i < numSpawners; ++i) {
    auto posOpt = map.FindRandomFreeCell();
    if (!posOpt.has_value()) break;
    spawners.push_back(std::make_unique<EnemySpawner>(posOpt.value()));
  }
  for (int i = 0; i < numTowers; ++i) {
    auto posOpt = map.FindRandomFreeCell();
    if (!posOpt.has_value()) break;
    towers.push_back(std::make_unique<Tower>(posOpt.value()));
  }
}

bool GameWorld::IsOccupied(int x, int y) const {
  Position pos{x, y};

  if (player.GetPosition() == pos) return true;
  for (auto& e : enemies)
    if (e && e->IsAlive() && e->GetPosition() == pos) return true;
  for (auto& s : spawners)
    if (s && s->IsAlive() && s->GetPosition() == pos) return true;
  for (auto& t : towers)
    if (t && t->IsAlive() && t->GetPosition() == pos) return true;
  for (auto& a : allies)
    if (a && a->IsAlive() && a->GetPosition() == pos) return true;
  return false;
}

GameMap& GameWorld::GetMap() { return map; }
Player& GameWorld::GetPlayer() { return player; }
std::vector<std::unique_ptr<Enemy>>& GameWorld::GetEnemies() { return enemies; }
std::vector<std::unique_ptr<EnemySpawner>>& GameWorld::GetSpawners() {
  return spawners;
}
std::vector<std::unique_ptr<Tower>>& GameWorld::GetTowers() { return towers; }

std::vector<std::unique_ptr<Trap>>& GameWorld::GetTraps() { return traps; }

std::vector<std::unique_ptr<SummonedAlly>>& GameWorld::GetAllies() {
  return allies;
}

void GameWorld::AddTrap(std::unique_ptr<Trap> t) {
  traps.push_back(std::move(t));
}

void GameWorld::AddSummonedAlly(std::unique_ptr<SummonedAlly> a) {
  allies.push_back(std::move(a));
}

void GameWorld::ResetForNextLevel(int newWidth, int newHeight, int enemyCount,
                                  int spawnerCount, int towerCount) {
  GameMap newMap(newWidth, newHeight);
  map = std::move(newMap);

  enemies.clear();
  spawners.clear();
  towers.clear();
  traps.clear();
  allies.clear();

  InitEntities(enemyCount, spawnerCount, towerCount);
  auto spawnPos = map.FindRandomFreeCell();
  if (spawnPos) {
    player.SetPosition(*spawnPos);
  }
}
int GameWorld::GetCurrentLevel() const { return currentLevel; }
void GameWorld::SetCurrentLevel(int next) { currentLevel = next; }