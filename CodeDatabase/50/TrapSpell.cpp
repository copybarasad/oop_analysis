#include "TrapSpell.h"
#include <algorithm>
#include <limits>

TrapSpell::TrapSpell(double damage, int range, int manaCost)
    : damage(damage), range(range), manaCost(manaCost) {}

bool TrapSpell::canCast(const Field& field, const Player& player) const {
    if (player.getMana() < manaCost) {
        std::cout << "Недостаточно маны для установки ловушки.\n";
        return false;
    }
    return true;
}

void TrapSpell::cast(Field& field, Player& player) {
    if (!canCast(field, player)) return;

    int px = player.getX();
    int py = player.getY();

    std::vector<std::pair<int, int>> possibleCells;

    // Собираем клетки в радиусе, куда можно поставить ловушку
    for (int y = std::max(0, py - range); y <= std::min(field.getHeight() - 1, py + range); ++y) {
        for (int x = std::max(0, px - range); x <= std::min(field.getWidth() - 1, px + range); ++x) {
            if (isValidPlacement(field, x, y))
                possibleCells.emplace_back(x, y);
        }
    }

    if (possibleCells.empty()) {
        std::cout << "Нет свободных клеток для установки ловушки.\n";
        return;
    }

    std::cout << "Выберите клетку для установки ловушки:\n";
    for (size_t i = 0; i < possibleCells.size(); ++i) {
        std::cout << i + 1 << ") (" << possibleCells[i].first << ", " << possibleCells[i].second << ")\n";
    }

    int choice;
    int choice_flag = 1;
    while (choice_flag) {
        std::cout << "Введите номер: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число.\n";
            continue;
        }
        if (choice >= 1 && choice <= (int)possibleCells.size()){
            choice_flag = 0;
        } else {
            std::cout << "Некорректный выбор.\n";
        }
        
    }

    int tx = possibleCells[choice - 1].first;
    int ty = possibleCells[choice - 1].second;

    // Добавляем ловушку
    field.setCellType(CellType::Trap, tx, ty);

    std::cout << "Вы установили ловушку на (" << tx << ", " << ty << ").\n";
    player.reduceMana(manaCost);
}

bool TrapSpell::isValidPlacement(Field& field, int x, int y){
    return field.cellIsEmpty(x,y);
}