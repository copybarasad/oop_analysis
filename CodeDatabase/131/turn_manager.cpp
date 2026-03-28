#include "turn_manager.h"
#include "game.h"
#include "game_event.h"
#include <algorithm>
#include <sstream>

namespace rpg {
    [[nodiscard]] std::string TurnManager::MovePlayer(Player &player, Direction dir,
                                                      const GameField &field, EntityManager &entities) {
        if (!player.CanMove()) {
            return "Игрок замедлен и не может двигаться в этот ход.";
        }

        const Position current_pos = player.GetPosition();
        size_t new_x, new_y;
        if (!GetNewPosition(current_pos.x, current_pos.y, dir, new_x, new_y, field)) {
            return "Невозможно переместиться в этом направлении (выход за границы).";
        }

        const Position new_pos(new_x, new_y);

        if (entities.IsPositionOccupiedByEnemy(new_pos)) {
            Enemy *enemy = entities.FindEnemyAt(new_pos);
            if (enemy != nullptr) {
                const int damage = player.GetDamage();
                enemy->TakeDamage(damage);

                std::ostringstream oss;
                oss << "Игрок атаковал врага на позиции (" << new_pos.x << ", " << new_pos.y
                        << "), нанеся " << damage << " урона.";

                if (!enemy->IsAlive()) {
                    player.AddScore(Game::kScorePerKill);
                    oss << " Враг уничтожен! Получено очков: " << Game::kScorePerKill << ".";
                }

                return oss.str();
            }
        }

        if (entities.IsPositionOccupiedByBuilding(new_pos)) {
            return "На этой клетке находится вражеское здание. Невозможно переместиться.";
        }

        if (!field.IsPassable(new_pos.x, new_pos.y)) {
            return "Эта клетка непроходима.";
        }

        player.SetPosition(new_pos);

        std::ostringstream oss;
        oss << "Игрок переместился на позицию (" << new_pos.x << ", " << new_pos.y << ").";

        if (field.IsSlowing(new_x, new_y)) {
            player.Slow();
            oss << " Клетка замедляет! Следующий ход пропущен.";
        }

        return oss.str();
    }

    [[nodiscard]] std::string TurnManager::ProcessEnemyTurns(EntityManager &entities, Player &player,
                                                             const GameField &field) {
        std::ostringstream messages;
        for (auto &enemy: entities.GetEnemies()) {
            std::string msg = MoveEnemy(enemy, player, entities, field);
            if (!msg.empty()) {
                messages << msg << "\n";
            }
        }
        return messages.str();
    }

    void TurnManager::ProcessBuildingTurns(EntityManager &entities, Game &game) {
        for (auto &building: entities.GetBuildings()) {
            auto maybe_enemy = building.TrySpawn(
                [&game, &entities](Position pos) {
                    return game.GetField().IsInBounds(pos.x, pos.y) &&
                           game.GetField().IsPassable(pos.x, pos.y) &&
                           !entities.IsPositionOccupiedByEnemy(pos) &&
                           pos != game.GetPlayerPosition() &&
                           !entities.IsPositionOccupiedByBuilding(pos);
                });

            if (maybe_enemy.has_value()) {
                entities.GetEnemies().push_back(std::move(maybe_enemy.value()));
            }
        }
    }

    [[nodiscard]] std::string TurnManager::ProcessAllyTurns(EntityManager &entities,
                                                            const GameField &field) {
        std::ostringstream messages;
        for (auto &ally: entities.GetAllies()) {
            if (!ally.IsAlive()) continue;
            std::string msg = MoveAlly(ally, entities, field);
            if (!msg.empty()) {
                messages << msg << "\n";
            }
        }
        return messages.str();
    }

    void TurnManager::ProcessTowerTurns(EntityManager &entities, Game &game) {
        Position player_pos = game.GetPlayerPosition();
        
        for (auto &tower: entities.GetTowers()) {
            tower.UpdateCooldown();
            
            if (tower.CanAttack(player_pos)) {
                game.DamagePlayer(tower.GetAttackDamage());
                tower.ResetCooldown();
            }
        }
    }

    [[nodiscard]] std::string TurnManager::CheckTraps(EntityManager &entities) {
        auto &traps = entities.GetTraps();
        auto &enemies = entities.GetEnemies();
        std::ostringstream messages;

        traps.erase(
            std::remove_if(traps.begin(), traps.end(),
                           [&enemies, &messages](const Trap &trap) {
                               for (auto &enemy: enemies) {
                                   if (enemy.GetPosition() == trap.GetPosition()) {
                                       enemy.TakeDamage(trap.GetDamage());
                                       messages << "Враг попал в ловушку! Урон: " << trap.GetDamage();
                                       if (!enemy.IsAlive()) {
                                           messages << " (убит!)";
                                       }
                                       messages << "\n";
                                       return true;
                                   }
                               }
                               return false;
                           }),
            traps.end()
        );
        return messages.str();
    }

    [[nodiscard]] std::string TurnManager::MoveEnemy(Enemy &enemy, Player &player,
                                                     EntityManager &entities, const GameField &field) {
        Position enemy_pos = enemy.GetPosition();
        Position player_pos = player.GetPosition();

        int dx = static_cast<int>(player_pos.x) - static_cast<int>(enemy_pos.x);
        int dy = static_cast<int>(player_pos.y) - static_cast<int>(enemy_pos.y);

        Direction dir;
        if (std::abs(dx) > std::abs(dy)) {
            dir = (dx > 0) ? Direction::kRight : Direction::kLeft;
        } else {
            dir = (dy > 0) ? Direction::kDown : Direction::kUp;
        }

        size_t new_x, new_y;
        if (GetNewPosition(enemy_pos.x, enemy_pos.y, dir, new_x, new_y, field)) {
            Position new_pos(new_x, new_y);

            if (new_pos == player_pos) {
                player.TakeDamage(enemy.GetDamage());
                return "Враг атаковал вас! Урон: " + std::to_string(enemy.GetDamage());
            }

            for (auto &ally: entities.GetAllies()) {
                if (ally.GetPosition() == new_pos && ally.IsAlive()) {
                    ally.TakeDamage(enemy.GetDamage());
                    std::ostringstream msg;
                    msg << "Враг атаковал союзника! Урон: " << enemy.GetDamage();
                    if (!ally.IsAlive()) {
                        msg << " (союзник погиб!)";
                    }
                    return msg.str();
                }
            }

            if (!entities.IsPositionOccupiedByEnemy(new_pos) &&
                !entities.IsPositionOccupiedByBuilding(new_pos) &&
                field.IsPassable(new_x, new_y)) {
                enemy.SetPosition(new_pos);
            }
        }
        return "";
    }

    [[nodiscard]] std::string TurnManager::MoveAlly(Ally &ally, EntityManager &entities,
                                                    const GameField &field) {
        auto &enemies = entities.GetEnemies();

        if (enemies.empty()) {
            return "";
        }

        Position ally_pos = ally.GetPosition();
        Position nearest_enemy_pos = enemies[0].GetPosition();
        int min_distance = std::abs(static_cast<int>(ally_pos.x) - static_cast<int>(nearest_enemy_pos.x)) +
                           std::abs(static_cast<int>(ally_pos.y) - static_cast<int>(nearest_enemy_pos.y));

        for (const auto &enemy: enemies) {
            if (!enemy.IsAlive()) continue;
            Position enemy_pos = enemy.GetPosition();
            int distance = std::abs(static_cast<int>(ally_pos.x) - static_cast<int>(enemy_pos.x)) +
                           std::abs(static_cast<int>(ally_pos.y) - static_cast<int>(enemy_pos.y));

            if (distance < min_distance) {
                min_distance = distance;
                nearest_enemy_pos = enemy_pos;
            }
        }

        int dx = static_cast<int>(nearest_enemy_pos.x) - static_cast<int>(ally_pos.x);
        int dy = static_cast<int>(nearest_enemy_pos.y) - static_cast<int>(ally_pos.y);

        Direction dir;
        if (std::abs(dx) > std::abs(dy)) {
            dir = (dx > 0) ? Direction::kRight : Direction::kLeft;
        } else {
            dir = (dy > 0) ? Direction::kDown : Direction::kUp;
        }

        size_t new_x, new_y;
        if (GetNewPosition(ally_pos.x, ally_pos.y, dir, new_x, new_y, field)) {
            Position new_pos(new_x, new_y);

            for (auto &enemy: enemies) {
                if (enemy.GetPosition() == new_pos && enemy.IsAlive()) {
                    enemy.TakeDamage(ally.GetDamage());
                    std::ostringstream msg;
                    msg << "Союзник атаковал врага! Урон: " << ally.GetDamage();
                    if (!enemy.IsAlive()) {
                        msg << " (враг убит!)";
                    }
                    return msg.str();
                }
            }

            if (!entities.IsPositionOccupiedByEnemy(new_pos) &&
                !entities.IsPositionOccupiedByBuilding(new_pos) &&
                field.IsPassable(new_x, new_y)) {
                bool occupied_by_ally = false;
                for (const auto &other_ally: entities.GetAllies()) {
                    if (other_ally.GetPosition() == new_pos && other_ally.IsAlive()) {
                        occupied_by_ally = true;
                        break;
                    }
                }

                if (!occupied_by_ally) {
                    ally.SetPosition(new_pos);
                }
            }
        }
        return "";
    }

    [[nodiscard]] bool TurnManager::GetNewPosition(size_t x, size_t y, Direction dir,
                                                   size_t &new_x, size_t &new_y,
                                                   const GameField &field) const {
        new_x = x;
        new_y = y;

        switch (dir) {
            case Direction::kUp:
                if (y > 0) {
                    new_y = y - 1;
                    return true;
                }
                break;
            case Direction::kDown:
                if (y < field.GetHeight() - 1) {
                    new_y = y + 1;
                    return true;
                }
                break;
            case Direction::kLeft:
                if (x > 0) {
                    new_x = x - 1;
                    return true;
                }
                break;
            case Direction::kRight:
                if (x < field.GetWidth() - 1) {
                    new_x = x + 1;
                    return true;
                }
                break;
        }

        return false;
    }
}
