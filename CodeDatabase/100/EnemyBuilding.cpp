#include "EnemyBuilding.h"
#include <iostream>
#include <sstream>
#include "../interface/ConsoleRenderer.h"
#include "../game_main/GameState.h"

// Конструктор
EnemyBuilding::EnemyBuilding(int health, int startX, int startY, int spawnInterval, char symbol) : 
                        health(health), posX(startX), posY(startY), symbol(symbol), spawnCounter(0), 
                        spawnInterval(spawnInterval) { }

// Спавн врагов
void EnemyBuilding::update(GameState& state, const GameField& field, int level) {
    if (!isAlive()) return;

    spawnCounter++; // Счётчик до спавна следующего врага
    if (spawnCounter < spawnInterval) return;   

    spawnCounter = 0;

    int directions[8][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};

    // Пытаемся заспавнить врага в 1 из 8 возможных клеток (вокруг башни)
    for (int i = 0; i < 8; i++) {
        int newX = posX + directions[i][0];
        int newY = posY + directions[i][1];

        if (!field.isCellPassable(newX, newY) || field.isCellTrap(newX, newY)) continue;

        bool occupied = false;
        for (const auto& character : state.getAllCharacters()) {
            if (character->isAlive() && character->getX() == newX && character->getY() == newY)
                occupied = true;
        }
        for (const auto& building : state.getAllBuildings()) {
            if (building->isAlive() && building->getX() == newX && building->getY() == newY)
                occupied = true;
        }
        for (const auto& tower : state.getAllTowers()) {
            if (tower->isAlive() && tower->getX() == newX && tower->getY() == newY)
                occupied = true;
        }
        for (const auto& ally : state.getAllAllies()) {
            if (ally->isAlive() && ally->getX() == newX && ally->getY() == newY)
                occupied = true;
        }
        
        if (occupied) continue;

        int baseHealth = 30;
        int baseDamage = 15;

        int health = baseHealth + 10 * (level - 1); 
        int damage = baseDamage + 5 * (level - 1);
        // Спавним врага, добавляет в allCharacters
        Enemy* newEnemy = new Enemy(health, damage, newX, newY, 'E');
        state.addCharacter(newEnemy);

        // Вывод информации о спавне
        std::ostringstream ss;
        ss << Colors::YELLOW << "Enemy building spawned new enemy at (" 
            << newX << ", " << newY << ")" << Colors::RESET;
        state.addMessage(ss.str());

        break; 
    }
}

void EnemyBuilding::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

// Геттеры
bool EnemyBuilding::isAlive() const { return health > 0; }

int EnemyBuilding::getHealth() const { return health; }

int EnemyBuilding::getX() const { return posX; }

int EnemyBuilding::getY() const { return posY; }

char EnemyBuilding::getSymbol() const { return symbol; }