#ifndef OOP_TURN_MANAGER_H_
#define OOP_TURN_MANAGER_H_

#include "entity_manager.h"
#include "game_field.h"
#include "player.h"

namespace rpg {
    class Game;

    enum class Direction;

    class TurnManager final {
    public:
        TurnManager() = default;

        [[nodiscard]] std::string MovePlayer(Player &player, Direction dir,
                                             const GameField &field, EntityManager &entities);

        [[nodiscard]] std::string ProcessEnemyTurns(EntityManager &entities, Player &player,
                                                    const GameField &field);

        void ProcessBuildingTurns(EntityManager &entities, Game &game);

        [[nodiscard]] std::string ProcessAllyTurns(EntityManager &entities, const GameField &field);

        void ProcessTowerTurns(EntityManager &entities, Game &game);

        [[nodiscard]] std::string CheckTraps(EntityManager &entities);

    private:
        [[nodiscard]] std::string MoveEnemy(Enemy &enemy, Player &player,
                                            EntityManager &entities, const GameField &field);

        [[nodiscard]] std::string MoveAlly(Ally &ally, EntityManager &entities,
                                           const GameField &field);

        [[nodiscard]] bool GetNewPosition(size_t x, size_t y, Direction dir,
                                          size_t &new_x, size_t &new_y,
                                          const GameField &field) const;
    };
}

#endif
