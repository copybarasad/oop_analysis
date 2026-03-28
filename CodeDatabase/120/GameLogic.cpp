#include "GameLogic.h"

GameLogic::GameLogic(GameWorld& w, EventSystem& es) : world(w), events(es) {}



void GameLogic::MovePlayer(int dx, int dy) {
  auto& player = world.GetPlayer();
  auto& map = world.GetMap();

  if (player.IsSlowed()) {
    events.Notify(GameMessageEvent{"Player skips turn."});
    player.ResetStatus();
    return;
  }

  Position pos = player.GetPosition();
  int nx = pos.x + dx, ny = pos.y + dy;

  if (!map.IsInside(nx, ny)) return;
  if (map.IsWalkable(nx, ny) && !world.IsOccupied(nx, ny)) {
    player.SetPosition({nx, ny});
    events.Notify(MoveEvent{"Player", nx, ny});
    if (map.GetCellType(nx, ny) == Cell::CellType::Slow) {
      player.SetSlowed();
      events.Notify(GameMessageEvent{"Stepped on slow cell! Next turn skipped."});
    }
  } else
    events.Notify(GameMessageEvent{"Can't move there. Cell blocked or occupied!"});
}

void GameLogic::PlayerAttack() {
  auto& player = world.GetPlayer();
  auto& enemies = world.GetEnemies();
  auto& spawners = world.GetSpawners();
  auto& towers = world.GetTowers();

  Position p = player.GetPosition();
  int range = (player.GetMode() == Player::CombatMode::Melee) ? 1 : 4;
  int damage = player.GetDamage();
  if (player.IsSlowed()) {
    events.Notify(GameMessageEvent{"Player skips turn."});
    player.ResetStatus();
    return;
  }

  auto tryAttack = [&](auto& container, int scorePerKill,
                       const std::string& name) {
    for (size_t i = 0; i < container.size(); ++i) {
      auto& target = container[i];
      if (!target || !target->IsAlive()) continue;

      int dist = std::abs(p.x - target->GetPosition().x) +
                 std::abs(p.y - target->GetPosition().y);
      if (dist <= range) {
        target->TakeDamage(damage);
        events.Notify(DamageEvent{name,damage,target->GetPosition().x,target->GetPosition().y});
        if (!target->IsAlive()) {
          events.Notify(GameMessageEvent{name + " defeated!"});
          player.AddScore(scorePerKill);
          container.erase(container.begin() + i);
          --i;
        }
        return true;
      }
    }
    return false;
  };

  bool hit = tryAttack(enemies, 10, "Enemy");
  if (!hit) hit = tryAttack(spawners, 20, "Spawner");
  if (!hit) hit = tryAttack(towers, 15, "Tower");
  if (!hit) events.Notify(GameMessageEvent{"No enemies in range."});
}

void GameLogic::UpdateEnemies() {
  auto& map = world.GetMap();
  auto& player = world.GetPlayer();
  auto& enemies = world.GetEnemies();

  const int DX[4] = {1, -1, 0, 0};
  const int DY[4] = {0, 0, 1, -1};
  int h = map.GetHeight(), w = map.GetWidth();

  for (auto& enemy : enemies) {
    if (!enemy || !enemy->IsAlive()) continue;
    Position ePos = enemy->GetPosition(), playerPos = player.GetPosition();

    if (std::abs(ePos.x - playerPos.x) + std::abs(ePos.y - playerPos.y) == 1) {
      player.TakeDamage(enemy->GetDamage());
      events.Notify(DamageEvent{"Enemy",enemy->GetDamage(), playerPos.x, playerPos.y});
      continue;
    }

    std::queue<Position> q;
    std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));
    std::vector<std::vector<Position>> parent(
        h, std::vector<Position>(w, {-1, -1}));

    q.push(ePos);
    visited[ePos.y][ePos.x] = true;
    bool found = false;
    Position targetStep = ePos;

    while (!q.empty() && !found) {
      Position cur = q.front();
      q.pop();
      for (int i = 0; i < 4; ++i) {
        int nx = cur.x + DX[i], ny = cur.y + DY[i];
        if (!map.IsInside(nx, ny) || !map.IsWalkable(nx, ny) ||
            visited[ny][nx] || world.IsOccupied(nx, ny))
          continue;
        visited[ny][nx] = true;
        parent[ny][nx] = cur;
        if (abs(nx - playerPos.x) + abs(ny - playerPos.y) == 1) {
          targetStep = {nx, ny};
          found = true;
          break;
        }
        q.push({nx, ny});
      }
    }

    if (found) {
      Position step = targetStep;
      while (!(parent[step.y][step.x] == ePos)) step = parent[step.y][step.x];
      enemy->SetPosition(step);
      events.Notify(MoveEvent{"Enemy", step.x, step.y});
    }
  }
}

void GameLogic::UpdateSpawners() {
  auto& map = world.GetMap();
  auto& enemies = world.GetEnemies();
  auto& spawners = world.GetSpawners();

  for (auto& spawner : spawners) {
    if (!spawner || !spawner->IsAlive()) continue;
    auto newEnemyTemplate = spawner->UpdateAndSpawn();
    if (!newEnemyTemplate) continue;
    auto freeCells = map.FindFreeAdjacent(spawner->GetPosition());
    if (freeCells.empty()) continue;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, (freeCells.size()) - 1);
    Position spawnPos = freeCells[dis(gen)];

    auto newEnemy = std::make_unique<Enemy>(
        newEnemyTemplate->GetHealth(), newEnemyTemplate->GetDamage(), spawnPos);
    enemies.push_back(std::move(newEnemy));
    events.Notify(GameMessageEvent{"Spawner created new enemy at (" + std::to_string(spawnPos.x) + "," +
           std::to_string(spawnPos.y) + ")"});
  }
}

bool GameLogic::IsGameOver() const {
  const auto& enemies = world.GetEnemies();
  const auto& spawners = world.GetSpawners();
  const auto& towers = world.GetTowers();
  return gameOver || (enemies.empty() && spawners.empty() && towers.empty());
}

bool GameLogic::WasLevelCleared() const { return levelCleared; }

void GameLogic::ClearLevelClearedFlag() { levelCleared = false; }

void GameLogic::ResetGameOverFlag() { gameOver = false; }

void GameLogic::UpdateTurn() {
  auto& player = world.GetPlayer();
  const auto& enemies = world.GetEnemies();
  const auto& spawners = world.GetSpawners();
  auto& traps = world.GetTraps();

  for (auto it = traps.begin(); it != traps.end();) {
    auto& trap = *it;
    if (!trap || !trap->IsAlive()) {
      ++it;
      continue;
    }

    Position tpos = trap->GetPosition();
    bool triggered = false;

    auto& enemies = world.GetEnemies();
    for (auto e_it = enemies.begin(); e_it != enemies.end(); ++e_it) {
      auto& e = *e_it;
      if (e && e->IsAlive() && e->GetPosition() == tpos) {
        e->TakeDamage(trap->GetDamage());
        events.Notify(DamageEvent{"Trap",trap->GetDamage(), tpos.x, tpos.y});

        if (!e->IsAlive()) {
          events.Notify(GameMessageEvent{"Enemy defeated by a trap!"});
          world.GetPlayer().AddScore(10);
          e_it = enemies.erase(e_it);
          --e_it;
        }

        triggered = true;
        break;
      }
    }

    if (triggered) {
      it = traps.erase(it);
    } else {
      ++it;
    }
  }

  auto& allies = world.GetAllies();
  for (size_t i = 0; i < allies.size(); ++i) {
    auto& ally = allies[i];
    if (!ally || !ally->IsAlive()) {
      allies.erase(allies.begin() + i);
      --i;
      continue;
    }
    ally->Act(world, *this);
  }
  UpdateEnemies();
  UpdateSpawners();
  auto& towers = world.GetTowers();
  for (auto& tower : towers) {
    if (tower) {
      tower->Update(world, *this);
    }
  }

  if (!player.IsAlive()) {
    events.Notify(GameMessageEvent{"Player died! Game over."});
    gameOver = true;
  } else if (enemies.empty() && spawners.empty() && towers.empty()) {
    events.Notify(GameMessageEvent{"All enemies and spawners defeated! YOU WIN!"});
    levelCleared = true;
  }
}

bool GameLogic::PlayerCastSpell(size_t handIndex,
                                const std::optional<Position>& targetPos) {
  auto& player = world.GetPlayer();
  auto& hand = player.GetHand();

  if (handIndex >= hand.Size()) {
    events.Notify(GameMessageEvent{"Invalid hand index"});
    return false;
  }

  auto spell = hand.TakeSpell(handIndex);
  if (!spell) {
    events.Notify(GameMessageEvent{"Empty slot"});
    return false;
  }

  bool used = spell->Use(world, *this, player.GetPosition(), targetPos);

  if (!used) {
    hand.AddSpell(std::move(spell));
    events.Notify(GameMessageEvent{"Spell failed or invalid target."});
    return false;
  }

  return true;
}
