#include "trap.h"
#include "player.h"
#include "game_field.h"
#include <iostream>
#include <limits>
#include <cmath>

Trap::Trap(int damage) : damage_(damage), active_(damage > 0) {}

int Trap::GetDamage() const { return damage_; }
bool Trap::IsActive() const { return active_; }
void Trap::Trigger() { active_ = false; }

void Trap::CastTrapSpell(Player& player, EnemyManager&, GameField& field) {
    std::cout << "Размещение ловушки. Выберите клетку (x y): ";
    int x, y;
    if (!(std::cin >> x >> y)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ввод! Заклинание отменено.\n";
        return;
    }

    Position trapPos = {x, y};

    int distance = std::abs(trapPos.x - player.GetPosition().x) + 
                   std::abs(trapPos.y - player.GetPosition().y);
    const int MAX_RANGE = 3;
    if (distance > MAX_RANGE) {
        std::cout << "Слишком далеко! Максимальная дистанция: " << MAX_RANGE << " клеток\n";
        return;
    }

    if (field.PlaceTrap(trapPos, 30)) {
        std::cout << "Ловушка установлена на позиции (" << x << ", " << y << ")!\n";
        std::cout << "Урон: 30, сработает при наступлении врага.\n";
    } else {
        std::cout << "Невозможно установить ловушку на эту клетку!\n";
    }
}