#include <limits>
#include "AreaDamageSpell.h"
#include "Field.h"

AreaDamageSpell::AreaDamageSpell(int damage)
    : damage_(damage) {}

std::string AreaDamageSpell::GetName() const {
    return "Заклинание урона по площади (2x2)";
}

std::pair<int, int> AreaDamageSpell::GetAreaTopLeft(int field_width, int field_height) const {
    int x = 0, y = 0;
    bool asking = true;

    while (asking) {
        std::cout << "Введите координату левого верхнего угла области для удара (x y): ";

        if (!(std::cin >> x >> y)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода! Нужно ввести два целых числа, например: 3 4\n";
            continue;
        }

        if (x >= 0 && y >= 0 && x + 1 < field_width && y + 1 < field_height) {
            asking = false; // координаты корректны — выходим из цикла
        } else {
            std::cout << "Некорректные координаты! Область 2x2 выходит за пределы поля.\n";
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return {x, y};
}

bool AreaDamageSpell::Use(std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& enemy_buildings,
                          int, int, Field& field) {
    bool any_hit = false;
    auto [x, y] = GetAreaTopLeft(field.GetWidth(), field.GetHeight());
    for (auto& enemy : enemies) {
        int ex = enemy.GetX();
        int ey = enemy.GetY();
        if ((ex == x || ex == x + 1) && (ey == y || ey == y + 1)) {
            enemy.TakeSpellDamage(damage_, false);
            any_hit = true;
        }
    }

    for (auto& building : enemy_buildings) {
        int ex = building.GetX();
        int ey = building.GetY();
        if ((ex == x || ex == x + 1) && (ey == y || ey == y + 1)) {
            building.TakeSpellDamage(damage_, false);
            any_hit = true;
        }
    }

    if (!any_hit) {
        std::cout << "В области 2x2 нет врагов или зданий! Вы теряете заклинание." << "\n";
    }
    return true;
}
