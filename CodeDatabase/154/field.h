#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <unordered_map>
#include "cell.h"
#include "entity.h"
#include "player.h"

enum class Direction { Up, Down, Left, Right, None };

class Field {
public:

    int generateEntityId();
    bool inBounds(int r, int c) const;
    bool getPosition(int id, int& r, int& c) const;
    int manhattanDistance(int r1, int c1, int r2, int c2) const;
    int getOccupantIdAt(int r, int c) const;
    bool isEnemy(int id) const;
    bool isBuilding(int id) const;
    Entity* getEntity(int id);
    const Entity* getEntity(int id) const;
    void removeEntity(int id);
    void placeEntity(Entity* e, int r, int c);
    bool cellIsEmpty(int r, int c) const;
    int findPlayerInRadius(int r, int c, int range) const;
    void storeBuffForNextSpell(int casterId, int extraRange, int extraArea, int extraTrapDamage, int extraSummons);
    Field(int rows, int cols);
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;
    int rows() const;
    int cols() const;
    bool placePlayer(Player* player, int row, int col);
    bool placeEnemy(Entity* enemy, int row, int col);
    bool placeBuilding(Entity* building, int row, int col);
    bool placeAlly(Entity* ally, int row, int col);
    bool movePlayer(Direction dir);
    void updateEntities();
    void print() const;
    bool isGameOver() const;
    void togglePlayerCombatMode();
    bool playerCastSpell(size_t index, int targetRow, int targetCol);
    Player* getPlayerById(int id) const;
    int getPlayerId() const;
    const std::unordered_map<int, Entity*>& getAllEntities() const;
    const std::unordered_map<int, std::pair<int,int>>& getAllPositions() const;

private:
    int findEmptyAdjacent(int r, int c) const;
    std::vector<std::vector<Cell>> grid_;
    std::unordered_map<int, Entity*> entities_;
    std::unordered_map<int, std::pair<int,int>> positions_;
    int nextId_;
    int playerId_;
    int turnCounter_;
};

#endif