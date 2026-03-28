#include <limits>
#include "TrapSpell.h"

std::string TrapSpell::GetName() const {
    return "Заклинание ловушки";
}

std::pair<int, int> TrapSpell::GetCoordsForTrap(Field& field) const {
    int x = 0, y = 0;
    bool asking = true;

    while (asking) {
        std::cout << "Введите координаты для установки ловушки (x y): ";

        if (!(std::cin >> x >> y)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода! Нужно ввести два целых числа, например: 3 4\n";
            continue;
        }

        if (x >= 0 && y >= 0 && x + 1 < field.GetWidth() && y + 1 < field.GetHeight() && field.IsCellEmpty(x, y)) {
            asking = false;
        } else {
            std::cout << "Некорректные координаты! Клетка занята или вне поля.\n";
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return {x, y};
}

bool TrapSpell::Use(std::vector<Enemy>&, std::vector<EnemyBuilding>&, int, int, Field& field) {
    auto [x, y] = GetCoordsForTrap(field);
    field.SetCellType(x, y, CellType::TRAP);
    std::cout << "Вы установили ловушку в клетке (" << x << ", " << y << ")!\n";
    return true;
}
