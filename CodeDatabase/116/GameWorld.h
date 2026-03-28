#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "../eneties/Enemy.h"
#include "../eneties/Building.h"
#include "../eneties/Trap.h"
#include "../eneties/Fortress.h"
#include "../eneties/Ally.h"
#include "../eneties/Player.h"
#include "GameField.h"
#include <random>

class GameWorld{
private:
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Ally>> allies;
    std::vector<std::unique_ptr<Trap>> traps;
    std::vector<std::unique_ptr<Fortress>> fortresses;
    std::unique_ptr<Building> tower;
    GameField field;
    std::unique_ptr<Player> player;  // Изменено на unique_ptr
    
public:
    GameWorld(int width = 15, int height = 15);
    ~GameWorld();
    
    const GameField& getField() const;
    GameField& getField();
    void setTower(int x, int y, int Health, int frequency);
    const Player& getPlayer() const;
    Player& getPlayer();
    const Building& getTower() const { return *tower; }
    Building& getTower() { return *tower; }

    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const;
    std::vector<std::unique_ptr<Enemy>>& getEnemies();
    
    const std::vector<std::unique_ptr<Ally>>& getAllies() const;
    std::vector<std::unique_ptr<Ally>>& getAllies();
    
    const std::vector<std::unique_ptr<Trap>>& getTraps() const;
    std::vector<std::unique_ptr<Trap>>& getTraps();
    
    const std::vector<std::unique_ptr<Fortress>>& getFortresses() const;
    std::vector<std::unique_ptr<Fortress>>& getFortresses();

    int getEnemiesCount() const;
    void moveCharacter(int oldX, int oldY, int newX, int newY, Character& person);
    Character* getVillain(std::pair<int, int> target);
    Character* getHero(std::pair<int, int> target);

    void clearWorld();
    
    // Метод для установки игрока (для загрузки и переходов между уровнями)
    void setPlayer(std::unique_ptr<Player> newPlayer) {
        player = std::move(newPlayer);
    }
    
    // Метод для извлечения игрока (без уничтожения)
    std::unique_ptr<Player> extractPlayer() {
        return std::move(player);
    }
};