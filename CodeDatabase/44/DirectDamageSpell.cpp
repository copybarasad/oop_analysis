#include <iostream>
#include <limits>
#include "DirectDamageSpell.h"
#include "EnemyBuilding.h"
#include "Enemy.h"

DirectDamageSpell::DirectDamageSpell(int damage, int radius) :
    damage_(damage),
    radius_(radius) {}

std::string DirectDamageSpell::GetName() const {
    return "Заклинание ближнего боя";
}

int DirectDamageSpell::GetTarget(int available_size) const {
    int choice = 0;
    bool asking = true;

    while (asking) {
        std::cout << "Выберите цель для удара (нужно ввести номер): ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Введите число!\n";
            continue;
        }

        if (choice >= 0 && choice < available_size) {
            asking = false;
        } else {
            std::cout << "Введите корректный номер!\n";
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return choice;
}


bool DirectDamageSpell::Use(std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& enemy_buildings,
                             int px, int py, Field&) {
    std::vector<Entity*> available_targets;
    for (auto& enemy : enemies) {
        int dx = abs(enemy.GetX() - px);
        int dy = abs(enemy.GetY() - py);
        int manhattan_dist = dx + dy;
        if (manhattan_dist <= radius_ && (dx == 0 || dy == 0)) {
            available_targets.push_back(&enemy);
        }
    }

    for (auto& building : enemy_buildings) {
        int dx = abs(building.GetX() - px);
        int dy = abs(building.GetY() - py);
        int manhattan_dist = dx + dy;
        if (manhattan_dist <= radius_ && (dx == 0 || dy == 0)) {
            available_targets.push_back(&building);
        }
    }

    if (available_targets.empty()) {
        std::cout << "Рядом нет врагов или зданий в радиусе действия заклинания (или они стоят по диогнали)!" << "\n";
        return false;
    }

    std::cout << "Цели в радиусе " << radius_ << ":\n";
    for (size_t i = 0; i < available_targets.size(); ++i) {
        std::cout << i << ") " << available_targets[i]->GetDescription() << "\n";
    }

    int choice = GetTarget(static_cast<int>(available_targets.size()));
    
    available_targets[choice]->TakeSpellDamage(damage_, true);
    return true;
}