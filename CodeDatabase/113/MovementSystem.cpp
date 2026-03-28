#include "MovementSystem.h"
#include "GameField.h"
#include "Player.h"
#include <iostream>

MovementSystem::MovementSystem(int startX, int startY)
    : m_positionX(startX), m_positionY(startY) {}

bool MovementSystem::move(int deltaX, int deltaY, GameField& field, Player& player) {
    int newX = m_positionX + deltaX;
    int newY = m_positionY + deltaY;

    if (!isValidMove(newX, newY, field)) {
        // Ќе выводим сообщение здесь - это делает Player
        return false;
    }

    m_positionX = newX;
    m_positionY = newY;

    handleTrapTrigger(newX, newY, field, player);
    return true;
}

bool MovementSystem::tryMoveTo(int newX, int newY, GameField& field, Player& player) {
    if (!isValidMove(newX, newY, field)) {
        return false;
    }

    m_positionX = newX;
    m_positionY = newY;

    handleTrapTrigger(newX, newY, field, player);
    return true;
}

int MovementSystem::getPositionX() const { return m_positionX; }
int MovementSystem::getPositionY() const { return m_positionY; }

void MovementSystem::setPosition(int x, int y) {
    m_positionX = x;
    m_positionY = y;
}

bool MovementSystem::isValidMove(int x, int y, const GameField& field) const {
    if (!field.isValidPosition(x, y)) {
        return false;
    }

    if (!field.isCellPassable(x, y)) {
        return false;
    }

    return true;
}

void MovementSystem::handleTrapTrigger(int x, int y, GameField& field, Player& player) {
    if (field.hasTrap(x, y)) {
        field.triggerTrap(x, y, player);
    }
}