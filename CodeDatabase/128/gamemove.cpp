#include "gamemodel.hpp"
#include "gamemove.hpp" 
#include "constants.hpp"
#include "event_dispatcher.hpp"    
#include "events.hpp"              
#include "entity_snapshot_factory.hpp" 
#include <iostream>
#include <cstdlib>
#include <queue>
#include <climits>
#include <algorithm>
using namespace configs;
EntitySnapshotFactory factory;

GameMove::GameMove(Game& game): game_(game) {}

EnemyUnit* GameMove::GetEnemyById(int id) {
    for (auto& e : game_.getEnemies()) if (e.id == id) return &e;
    return nullptr;
}

bool GameMove::HandleRangedAttack(int x, int y, int dx, int dy) {
    auto& player = game_.getPlayer();
    auto& field = game_.getField();
    if (!player.isRanged() || (dx == 0 && dy == 0))
        return false;
    int nx = x + dx;
    int ny = y + dy;
    if (!game_.InBounds(nx, ny))
        return false;
    int rx = x + 2 * dx;
    int ry = y + 2 * dy;
    if (!game_.InBounds(rx, ry))
        return false;
    auto& nearCell = field.At(nx, ny);
    if (nearCell.getType() == CellType::kWall ||
        nearCell.getType() == CellType::kBuilding) {
        return false;
    }
    auto& distantCell = field.At(rx, ry);
    if (distantCell.getType() == CellType::kEnemy) {
        if (auto* e = GetEnemyById(distantCell.getEntityId())) {
            return AttackEnemy(e, rx, ry); 
        }
    }
    return false;
}

bool GameMove::HandleMeleeAttack(int nx, int ny) {
    auto& field = game_.getField();
    auto& cell = field.At(nx, ny);
    if (cell.getType() != CellType::kEnemy)
        return false;
    if (auto* e = GetEnemyById(cell.getEntityId()))
        return AttackEnemy(e, nx, ny);
    return false;
}


bool GameMove::AttackEnemy(EnemyUnit* e, int targetX, int targetY) {
    if (!e)
        return false;
    auto& player = game_.getPlayer();
    auto& field  = game_.getField();
    int oldHp = e->enemy.getHealth();
    player.Attack(e->enemy);
    int newHp = e->enemy.getHealth();
    int damage = std::max(0, oldHp - newHp);
    auto attackerSnap = factory.MakePlayerSnapshot(game_);
    auto targetSnap   = factory.MakeEnemySnapshot(*e);
    EventDispatcher::instance().notify(DamageEvent(attackerSnap, targetSnap, damage, oldHp, newHp));
    if (!e->enemy.isAlive()) {
        field.At(targetX, targetY).clear();
        EventDispatcher::instance().notify(
            DeadEvent(targetSnap)
        );
        game_.RewardPlayer(RewardType::KillEnemy);
    }

    return true;
}

bool GameMove::MovePlayer(int dx, int dy) {
    auto& player = game_.getPlayer();
    auto& field  = game_.getField();

    if (player.isSlowed()) {
        EventDispatcher::instance().notify(UiMessageEvent("🔍 The Mad Scientist is slowed and misses a turn!"));
        player.setSlowed(false);
        return true;
    }

    int x  = player.getX();
    int y  = player.getY();
    int nx = x + dx;
    int ny = y + dy;
    if (!game_.InBounds(nx, ny))
        return false;
    if (HandleRangedAttack(x, y, dx, dy))
        return true;
    CellType dest = field.At(nx, ny).getType();
    if (dest == CellType::kEnemy) {
        if (HandleMeleeAttack(nx, ny))
            return true;
    }
    if (dest == CellType::kEmpty || dest == CellType::kSlow) {
        auto fromCell = field.At(x, y).getType();
        auto toCell   = dest;
        field.At(nx, ny).set(CellType::kPlayer, -1);
        field.At(x, y).clear();
        player.setPosition(nx, ny);
        auto snap = factory.MakePlayerSnapshot(game_);
        EventDispatcher::instance().notify(MoveEvent(snap, {x, y}, {nx, ny}, fromCell, toCell));
        if (dest == CellType::kSlow) {
            player.setSlowed(true);
            EventDispatcher::instance().notify(UiMessageEvent("🌀 The Mad Scientist stepped on a slowing cell!"));
        }
        return true;
    }
    if (dest == CellType::kWall)
        return false;

    return false;
}

std::vector<std::vector<int>> GameMove::ComputeDistances(int playerX, int playerY) const {
    auto& field = game_.getField();
    std::vector<std::vector<int>> dist(field.getHeight(), std::vector<int>(field.getWidth(), INT_MAX));
    std::queue<std::pair<int, int>> q;
    dist[playerY][playerX] = 0; 
    q.push({playerX, playerY});
    static const int dx[kNumberSides] = {1, -1, 0, 0};
    static const int dy[kNumberSides] = {0, 0, 1, -1};
    while (!q.empty()) {
        auto p = q.front();
        int x = p.first;
        int y = p.second;
        q.pop();
        for (int i = 0; i < kNumberSides; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (!game_.InBounds(nx, ny))
                continue;
            auto& cell = field.At(nx, ny);
            if (cell.getType() == CellType::kWall || cell.getType() == CellType::kBuilding)
                continue;
            if (dist[ny][nx] > dist[y][x] + 1) {
                dist[ny][nx] = dist[y][x] + 1;
                q.push({nx, ny});
            }
        }
    }
    return dist;
}

void GameMove::MoveEnemies() {
    auto& player = game_.getPlayer();
    auto& field = game_.getField();
    auto& enemies = game_.getEnemies();
    int playerX = player.getX();
    int playerY = player.getY();
    auto dist = ComputeDistances(playerX, playerY);
    static const int dx[kNumberSides] = {1, -1, 0, 0};
    static const int dy[kNumberSides] = {0, 0, 1, -1};
    for (auto& e : enemies) {
        if (!e.enemy.isAlive())
            continue;
        int ex = e.x, ey = e.y;
        int bestX = ex, bestY = ey;
        int bestDist = dist[ey][ex];
        for (int i = 0; i < kNumberSides; ++i) {
            int nx = ex + dx[i];
            int ny = ey + dy[i];
            if (!game_.InBounds(nx, ny))
                continue;
            if (dist[ny][nx] != INT_MAX &&
                dist[ny][nx] < bestDist &&
                (field.At(nx, ny).getType() == CellType::kEmpty ||
                 field.At(nx, ny).getType() == CellType::kPlayer ||
                 field.At(nx, ny).getType() == CellType::kTrap ||
                 field.At(nx, ny).getType() == CellType::kAlly)) {
                bestDist = dist[ny][nx];
                bestX = nx;
                bestY = ny;
            }
        }
        ResolveEnemyCell(e, ex, ey, bestX, bestY, playerX, playerY);
    }
}

void GameMove::ResolveEnemyCell(EnemyUnit& e, int ex, int ey, int bestX, int bestY, int playerX, int playerY) {
    auto& field  = game_.getField();
    auto& player = game_.getPlayer();
    auto& allies = game_.getAllies();
    auto& traps  = game_.getTraps();
    CellType type = field.At(bestX, bestY).getType();
    bool trapTriggered = false;
    if (type == CellType::kTrap) {
        for (auto& trapPtr : traps) {
            auto& trap = *trapPtr;
            if (trap.IsActive() && trap.getX() == bestX && trap.getY() == bestY) {
                EventDispatcher::instance().notify(UiMessageEvent("💥 Trap triggered at (" + std::to_string(bestX) + "," + std::to_string(bestY) + ")!"));
                int oldHp   = e.enemy.getHealth();
                int damage  = trap.getDamage();
                e.enemy.TakeDamage(damage);
                int newHp   = e.enemy.getHealth();
                EntitySnapshot attackerSnap(-1, EntityType::Trap, trap.getDamage(), trap.getDamage(), {bestX, bestY});
                auto targetSnap = factory.MakeEnemySnapshot(e);
                EventDispatcher::instance().notify(DamageEvent(attackerSnap, targetSnap, damage, oldHp, newHp));
                trap.Deactivate();
                field.At(bestX, bestY).clear();
                if (!e.enemy.isAlive()) {
                    field.At(ex, ey).clear();
                    EventDispatcher::instance().notify(
                        DeadEvent(targetSnap)
                    );
                    game_.RewardPlayer(RewardType::KillEnemy);
                    return;
                } else {
                    EventDispatcher::instance().notify(UiMessageEvent("👽 Alien HP = " + std::to_string(e.enemy.getHealth())));
                }
                trapTriggered = true;
                break;
            }
        }
    }
    if (type == CellType::kAlly) {
        for (auto& ally : allies) {
            if (ally && ally->getX() == bestX && ally->getY() == bestY && ally->isAlive()) {
                int oldHp = ally->getHealth();
                e.enemy.Attack(*ally);
                int newHp = ally->getHealth();
                int dmg   = std::max(0, oldHp - newHp);
                auto attackerSnap = factory.MakeEnemySnapshot(e);
                EntitySnapshot targetSnap(-1, EntityType::Ally, newHp, ally->getDamage(), {bestX, bestY});
                EventDispatcher::instance().notify(DamageEvent(attackerSnap, targetSnap, dmg, oldHp, newHp));
                EventDispatcher::instance().notify(
                    UiMessageEvent("👽 Alien attacks ally at (" + std::to_string(bestX) + "," + std::to_string(bestY) +")! Ally HP = " + std::to_string(newHp)));
                if (!ally->isAlive()) {
                    field.At(bestX, bestY).clear();
                    EventDispatcher::instance().notify(DeadEvent(targetSnap));
                    EventDispatcher::instance().notify(UiMessageEvent("💀 Ally destroyed!"));
                }
                break;
            }
        }
        return;
    }
    if (bestX == playerX && bestY == playerY) {
        int oldHp = player.getHealth();
        e.enemy.Attack(player);
        int newHp = player.getHealth();
        int dmg   = std::max(0, oldHp - newHp);
        auto attackerSnap = factory.MakeEnemySnapshot(e);
        auto targetSnap   = factory.MakePlayerSnapshot(game_);
        EventDispatcher::instance().notify(DamageEvent(attackerSnap, targetSnap, dmg, oldHp, newHp));
        EventDispatcher::instance().notify(UiMessageEvent("🩸 The alien attacks! Mad Scientist HP = " + std::to_string(newHp)));
        return;
    }
    if (type == CellType::kEmpty || trapTriggered) {
        field.At(ex, ey).clear();
        field.At(bestX, bestY).set(CellType::kEnemy, e.id);
        e.x = bestX;
        e.y = bestY;
        auto snap = factory.MakeEnemySnapshot(e);
        EventDispatcher::instance().notify(
            MoveEvent(snap, {ex, ey}, {bestX, bestY},
                      CellType::kEnemy, type)
        );
    }
}

void GameMove::MoveAllies() {
    auto& field = game_.getField();
    auto& allies = game_.getAllies();
    for (auto& ally : allies) {
        if (!ally->isAlive()) {  
            field.At(ally->getX(), ally->getY()).clear(); 
            continue;
        }
        EnemyUnit* target = FindClosestEnemy(*ally); 
        if (!target) continue;
        int dist = std::abs(target->x - ally->getX()) +
                   std::abs(target->y - ally->getY());
        if (dist == 1) {
            AllyAttackEnemy(*ally, *target); 
        } else {
            MoveAllyTowards(*ally, *target); 
        }
    }
}

EnemyUnit* GameMove::FindClosestEnemy(const Ally& ally) {
    auto& enemies = game_.getEnemies();
    EnemyUnit* target = nullptr;
    int bestDist = INT_MAX;
    for (auto& e : enemies) {
        if (!e.enemy.isAlive()) continue;
        int dist = std::abs(e.x - ally.getX()) + std::abs(e.y - ally.getY());
        if (dist < bestDist) {
            bestDist = dist;
            target = &e;
        }
    }
    return target;
}

void GameMove::AllyAttackEnemy(Ally& ally, EnemyUnit& enemy) {
    auto& field = game_.getField();
    int oldHp  = enemy.enemy.getHealth();
    int damage = ally.getDamage();
    enemy.enemy.TakeDamage(damage);
    int newHp  = enemy.enemy.getHealth();
    EntitySnapshot attackerSnap(
        -1,
        EntityType::Ally,
        ally.getHealth(),  
        ally.getDamage(), 
        {ally.getX(), ally.getY()}
    );
    auto targetSnap = factory.MakeEnemySnapshot(enemy);
    EventDispatcher::instance().notify(DamageEvent(attackerSnap, targetSnap, damage, oldHp, newHp));
    EventDispatcher::instance().notify(
        UiMessageEvent("🤖 Ally attacks alien at (" + std::to_string(enemy.x) + "," + std::to_string(enemy.y) + ") for " + std::to_string(damage) + " damage!")
    );
    if (!enemy.enemy.isAlive()) {
        field.At(enemy.x, enemy.y).clear();
        EventDispatcher::instance().notify(DeadEvent(targetSnap));
        EventDispatcher::instance().notify( UiMessageEvent("☠️ Alien defeated by Ally!"));
    }
}

void GameMove::MoveAllyTowards(Ally& ally, const EnemyUnit& target) {
    static const int dx4[kNumberSides] = {1, -1, 0, 0};
    static const int dy4[kNumberSides] = {0, 0, 1, -1};
    auto& field = game_.getField();
    int ax = ally.getX();
    int ay = ally.getY();
    auto dist = ComputeDistances(target.x, target.y);
    int bestX = ax;
    int bestY = ay;
    int bestScore = dist[ay][ax];
    for (int i = 0; i < kNumberSides; ++i) {
        int nx = ax + dx4[i];
        int ny = ay + dy4[i];
        if (!game_.InBounds(nx, ny)) continue;
        auto type = field.At(nx, ny).getType();
        if (type == CellType::kWall || type == CellType::kBuilding)
            continue;
        if (dist[ny][nx] < bestScore &&
            (type == CellType::kEmpty || type == CellType::kTrap)) {
            bestScore = dist[ny][nx];
            bestX = nx;
            bestY = ny;
        }
    }
    if (bestX != ax || bestY != ay) {
        field.At(ax, ay).clear();
        field.At(bestX, bestY).set(CellType::kAlly, -1);
        ally.setPosition(bestX, bestY);
    }
}
