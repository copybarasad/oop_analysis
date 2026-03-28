#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <optional>
#include <functional>
#include "Cell.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Player.h"
#include "Pet.h"
#include "interfaces/IPlayerField.h"
#include "interfaces/IEnemyField.h"
#include "interfaces/IEnemyBuildingField.h"
#include "interfaces/IHandField.h"
#include "interfaces/IPetField.h"

class Entity;

class GameField : public IPlayerField,
                  public IEnemyField,
                  public IEnemyBuildingField,
                  public IHandField,
                  public IPetField {
private:
    int width, height;

    std::vector<std::vector<Cell>> field;
    std::vector<Enemy> enemies;
    std::vector<EnemyBuilding> buildings;
    std::vector<Pet> pets;
    Player player;
public:
    GameField(int w, int h);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    std::vector<std::vector<Cell>>& getCells() override { return field; }
    const std::vector<std::vector<Cell>>& getCells() const override { return field; }
    std::vector<Enemy>& getEnemies() override { return enemies; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    std::vector<EnemyBuilding>& getBuilding() override { return buildings; }
    const std::vector<EnemyBuilding>& getBuilding() const { return buildings; }
    std::vector<Pet>& getPets() override { return pets; }
    const std::vector<Pet>& getPets() const { return pets; }
    Player& getPlayer() override { return player; }
    const Player& getPlayer() const override { return player; }

    bool isValidPosition(int x, int y) const override;
    bool isPositionEmpty(int x, int y) const override;
    bool isPositionPassable(int x, int y) const override;
    std::optional<std::reference_wrapper<Entity>> findEntityAt(int x, int y) override;
    
    void removeDeadEntities();
};

#endif
