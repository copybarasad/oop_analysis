#pragma once
#include <vector>
#include <unordered_set>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>
#include <string>
#include <queue>
#include <numeric>
#include <limits>

#include "./states/CoupreAnimate.hpp"
#include "./states/DecelerationEffect.hpp"
#include "./states/AttackEffect.hpp"
#include "./states/IState.hpp"

#include "EntityManager.hpp"
#include "FieldCell.hpp"

#include "entities/Entity.hpp"
#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include "entities/EnemyBarracks.hpp"
#include "entities/EnemyTower.hpp"
#include "SaveData.hpp"
#include "Config.hpp"

#define MAX_FIELD_SIZE 70
#define MIN_FIELD_SIZE 10

#define GAMEFIELD_SAVES_DIR "/Game/GameField/"

class GameField {
private:
    std::vector<FieldCell> cells{};
    EntityManager entityManager{};
    int widthField;
    int heightField;
    int gameLevel;
    int gameTurn;
private:
    void generateFieldCells(std::unique_ptr<Entity> player);
    void moveEntity(int oldIndex, int newIndex);
    bool isMoveCorrect(int oldIndex, int newIndex);
    void spawnEntity(std::unique_ptr<Entity> entity, int index);
    void generateEnemy();
    int firstEnemyIndexOnLine(int oldIndex, int newIndex);
    int getBestTurnForEnemy(int enemyIndex, int playerIndex, std::unordered_map<int, int>& grid);
    std::vector<std::pair<int, float>> getDistanceToPlayer(std::vector<int> enemyIndexes, int playerIndex);
    bool enemyOnIndex(int index) const;
public:
    GameField();
    GameField(std::unique_ptr<Entity> player, int weight, int height, int gameLevel);
    std::unique_ptr<Entity> returnPlayer();
    bool playerAlive() const;
    std::shared_ptr<PlayerData> getPlayerData();
    std::vector<EnemyData> getEnemyData();
    bool playerTurn(char command);
    void enemyTurn();
    void buildingsTurn();
    int getCountOfEnemy();
    void playerLevelUp(char attribute);
    void update();
    std::vector<wchar_t> show();
    void animateBowAttack(int playerIndex, int enemyIndex);
    void animateSwordAttack(int enemyIndex);
    FieldSaveData getFieldSaveData();
    void setFieldSaveData(FieldSaveData data);
};
