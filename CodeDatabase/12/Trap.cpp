#include "Trap.h"
#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include <iostream>

Trap::Trap(int damage) : Damage(damage) {}

void Trap::cast(Player& player, std::vector<Enemy>& enemies, Field& field) {
    std::cout << "Вы устанавливаете ловушку!\n";
    std::cout << "Введите координаты для ловушки (x y): ";
    
    int trapX, trapY;
    std::cin >> trapX >> trapY;

    if (trapX < 0 || trapY < 0 || trapX >= field.getWidth() || trapY >= field.getHeight()) {
        std::cout << "Координаты вне поля!\n";
        return;
    }

    if (!field.CanMove(trapX, trapY)) {
        std::cout << "Нельзя установить ловушку в эту клетку!\n";
        return;
    }

    if (trapX == player.getX() && trapY == player.getY()) {
        std::cout << "Нельзя устанавливать ловушку под себя!\n";
        return;
    }

    field.setTrap(trapX, trapY, Damage);
    std::cout << "Ловушка готова! Урон: " << Damage << "\n";
}

std::string Trap::getName() const {
    return "Ловушка";
}