#include "game_object.hpp"
#include "map.hpp"
#include <stdexcept>
#include <iostream>

GameObject::GameObject(int startX, int startY, Cell& cell, int hp, int range)
    : positionX(startX), positionY(startY), currentCell(&cell), 
      healthPoints(hp), maxHealthPoints(hp), movementRange(range),
      isImmovable(movementRange < 1) {
    std::cout << "DEBUG: Создаём GameObject в (" << startX << "," << startY 
              << ") HP=" << hp << "\n";
    
    if (!cell.isEmptyCell()) {
        throw std::runtime_error("Cannot place object on occupied cell");
    }
    cell.setEmpty(false);
}

GameObject::~GameObject() {
    if (currentCell && isAlive()) {
        currentCell->setEmpty(true);
    }
}

GameObject::GameObject(GameObject&& other) noexcept
    : positionX(other.positionX), positionY(other.positionY),
      currentCell(other.currentCell),
      healthPoints(other.healthPoints),
      maxHealthPoints(other.maxHealthPoints),
      movementRange(other.movementRange),
      isImmovable(other.isImmovable) {
    other.currentCell = nullptr;
    other.positionX = -1;
    other.positionY = -1;
    other.healthPoints = 0;
}

GameObject& GameObject::operator=(GameObject&& other) noexcept {
    if (this != &other) {
        if (currentCell && isAlive()) {
            currentCell->setEmpty(true);
        }
        
        positionX = other.positionX;
        positionY = other.positionY;
        currentCell = other.currentCell;
        healthPoints = other.healthPoints;
        maxHealthPoints = other.maxHealthPoints;
        movementRange = other.movementRange;
        isImmovable = other.isImmovable;
        
        other.currentCell = nullptr;
        other.positionX = -1;
        other.positionY = -1;
        other.healthPoints = 0;
    }
    return *this;
}

int GameObject::getPositionX() const {
    return positionX;
}

int GameObject::getPositionY() const {
    return positionY;
}

int GameObject::getHealthPoints() const {
    return healthPoints;
}

int GameObject::getMaxHealthPoints() const {
    return maxHealthPoints;
}

void GameObject::takeDamage(int damage) {
    if (damage < 0) {
        throw std::invalid_argument("Урон должен быть положительный");
    }
    healthPoints -= damage;
    if (healthPoints < 0) healthPoints = 0;
    
    if (!isAlive() && currentCell) {
        currentCell->setEmpty(true);
    }
}

void GameObject::heal(int health) {
    if (health < 0) {
        throw std::invalid_argument("Лечение должно быть положительным");
    }
    healthPoints += health;
    if (healthPoints > maxHealthPoints) healthPoints = maxHealthPoints;
}

bool GameObject::isAlive() const {
    return healthPoints > 0;
}

bool GameObject::moveTo(int newX, int newY, Map& map) {
    if (!canMoveTo(newX, newY, map)) {
        return false;
    }
    
    Cell& newCell = map.getCell(newX, newY);
    updatePosition(newX, newY, newCell);
    return true;
}

Cell& GameObject::getCurrentCell() {
    if (!currentCell) {
        throw std::runtime_error("Не удалось правильно инициализировать");
    }
    return *currentCell;
}

bool GameObject::canMoveTo(int x, int y, Map& map) const {
    if (isImmovable) return false;
    return map.isValidPosition(x, y) && map.isCellPassable(x, y) && 
           map.getCell(x, y).isEmptyCell();
}

void GameObject::updatePosition(int newX, int newY, Cell& newCell) {
    if (currentCell) {
        currentCell->setEmpty(true);
    }
    currentCell = &newCell;
    positionX = newX;
    positionY = newY;
    currentCell->setEmpty(false);
}