#pragma once

#include <memory>
#include <vector>
#include "game_field.h"
#include "player.h"
#include "enemy.h"
#include "tower.h"
#include "hand.h"
#include "basespell.h"
#include "Logger.h"


#include <cstdlib>   // для rand()
#include <ctime>     // для srand()


// Вперёд объявляем классы, чтобы не тянуть лишние include'ы.
class GameField;
class Player;
class Enemy;
class SpawnerTower;
class Hand;

class GameController {
public:
    // 
    GameController(Player& player,
        Hand& hand,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        std::vector<std::unique_ptr<SpawnerTower>>& towers,
        unsigned int seed = 0,
        int levelIndex = 0,
        EventBus* bus = nullptr);

    // Запуск (или перезапуск) уровня.
    // Размер поля и сложность определяются по _levelIndex.
    bool startLevel();

    // Ход игрока — перегрузки
    void turnPlayer(Direction dir);                   // движение
    void turnPlayer(const Position& targetPos, const std::string& actionType, int i_spell); // атака или заклинание
    //найти ближайших врагов
    std::vector<Enemy*> getEnemiesInRadius(const Position& center, int radius);

    //ход врагов
    void turnEnemies();

    //ход башни
    void turnTowers();

    //очистка мёртвых юнитов
    int cleanupDeadUnits();


    int getLevelIndex() const { return _levelIndex; }
    int getSeed() const { return _seed; }
    const GameField* getField() const { return _field.get(); }

    // Вспомогательный метод для первичной генерации врагов/башен,
    void generateDefaultLevelContent();

private:
    Player& _player;                 // игрок создаётся извне
    Hand& _hand;                     // рука заклинаний игрока
    std::unique_ptr<GameField> _field;               // поле уровня
    std::vector<std::unique_ptr<Enemy>>& _enemies;
    std::vector<std::unique_ptr<SpawnerTower>>& _towers;

    int _seed;
    int _levelIndex;                             // текущий индекс уровня
    EventBus* _bus = nullptr;                    // не владеет
    
};
