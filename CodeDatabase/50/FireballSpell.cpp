#include "FireballSpell.h"
#include <iostream>
#include <algorithm>
#include <limits>

FireballSpell::FireballSpell(double damage, int range, int manaCost)
    : damage(damage), range(range), manaCost(manaCost) {}

bool FireballSpell::canCast(const Field& field, const Player& player) const {
    if (player.getMana() < manaCost) {
        std::cout << "Недостаточно маны.\n";
        return false;
    }

    int px = player.getX();
    int py = player.getY();

    for (int y = std::max(0, py - range); y <= std::min(field.getHeight() - 1, py + range); ++y) {
        for (int x = std::max(0, px - range); x <= std::min(field.getWidth() - 1, px + range); ++x) {
            GameEnt* ent = field.getCellEntity(x, y);
            if (ent && (ent->getObjType() == GameEntType::Enemy || ent->getObjType() == GameEntType::Tower))
                return true;
        }
    }

    std::cout << "Нет врагов в радиусе заклинания.\n";
    return false;
}

void FireballSpell::cast(Field& field, Player& player) {
    if (player.getMana() < manaCost) {
        std::cout << "Недостаточно маны!\n";
        return;
    }

    int px = player.getX();
    int py = player.getY();
    std::vector<std::pair<int, int>> targets;

    for (int y = std::max(0, py - range); y <= std::min(field.getHeight() - 1, py + range); ++y) {
        for (int x = std::max(0, px - range); x <= std::min(field.getWidth() - 1, px + range); ++x) {
            GameEnt* ent = field.getCellEntity(x, y);
            if (ent && (ent->getObjType() == GameEntType::Enemy || ent->getObjType() == GameEntType::Tower))
                targets.push_back({x, y});
        }
    }

    if (targets.empty()) {
        std::cout << "Нет подходящих целей.\n";
        return;
    }

    std::cout << "Выберите цель для атаки:\n";
    for (size_t i = 0; i < targets.size(); ++i)
        std::cout << i + 1 << ") (" << targets[i].first << ", " << targets[i].second << ")\n";

    int choice;
    int choice_flag = 1;
    while (choice_flag) {
        std::cout << "Введите номер цели: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число!\n";
            continue;
        }
        if (choice >= 1 && choice <= (int)targets.size()) {
            choice_flag = 0;
        } else {
            std::cout << "Некорректный номер, попробуйте снова.\n";
        }
        
    }

    auto [tx, ty] = targets[choice - 1];
    GameEnt* target = field.getCellEntity(tx, ty);
    if (!target) return;

    target->takeDamage(damage);
    player.reduceMana(manaCost);

    if (target->getObjType() == GameEntType::Enemy){
        std::cout << "Вы нанесли " << damage << " урона врагу (" << tx << ", " << ty << ").\n";
    } else {
        std::cout << "Вы нанесли " << damage << " урона башне (" << tx << ", " << ty << ").\n";
    }
    
    if (!target->isAlive()) {
        field.clearCellEnt(tx, ty);
    }
}


