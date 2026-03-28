#ifndef LEVELBUILDER_H
#define LEVELBUILDER_H


#include "EntityFactory.h"
#include "Game.h"
#include "LevelManager.h"


/**
 * @file LevelBuilder.h
 * @brief Строитель уровней
 * 
 * Отвечает за создание поля, размещение игрока и спавн сущностей
 * на основе конфигурации уровня.
 */
class LevelBuilder {
private:
    Game* game;  ///< Указатель на игру
    
public:
    /**
     * @brief Конструктор
     * @param game Указатель на игру
     */
    explicit LevelBuilder(Game* game) : game(game) {}
    
    /**
     * @brief Строит уровень по конфигурации
     * @param config Конфигурация уровня
     */
    void buildLevel(const LevelConfig& config) {
        createField(config);
        positionPlayer(config);
        spawnEnemies(config);
        spawnTowers(config);
    }


private:
    /**
     * @brief Создаёт игровое поле
     */
    void createField(const LevelConfig& config) {
        game->field = std::make_unique<GameField>(
            config.fieldWidth,
            config.fieldHeight
        );
        game->field->generateBlockedCells(config.blockedCellsCount);
    }


    /**
     * @brief Размещает игрока в центре поля
     */
    void positionPlayer(const LevelConfig& config) {
        int centerX = config.fieldWidth / 2;
        int centerY = config.fieldHeight / 2;
        game->player->setPosition(centerX, centerY);
    }


    /**
     * @brief Спавнит врагов
     */
    void spawnEnemies(const LevelConfig& config) {
        for (int i = 0; i < config.enemyCount; ++i) {
            auto enemy = EntityFactory::createEnemy(
                game->field.get(),
                config.enemyHealthBonus,
                config.enemyDamageBonus
            );
            
            if (enemy) {
                game->enemies.push_back(std::move(enemy));
            }
        }
    }


    /**
     * @brief Спавнит башни (ИСПРАВЛЕНО - добавлен бонус урона)
     */
    void spawnTowers(const LevelConfig& config) {
        for (int i = 0; i < config.towerCount; ++i) {
            auto tower = EntityFactory::createTower(
                game->field.get(),
                config.levelNumber * 20,  // health bonus
                config.levelNumber * 10   // ← ДОБАВЛЕН damage bonus
            );
            
            if (tower) {
                game->towers.push_back(std::move(tower));
            }
        }
    }
};


#endif // LEVELBUILDER_H


