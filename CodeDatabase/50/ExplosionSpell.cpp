#include "ExplosionSpell.h"
#include <iostream>
#include <algorithm>
#include <limits>

ExplosionSpell::ExplosionSpell(double damage, int range, int manaCost)
    : damage(damage), range(range), areaSize(1), manaCost(manaCost) {}

bool ExplosionSpell::canCast(const Field& field, const Player& player) const {
    if (player.getMana() < manaCost) {
        std::cout << "Недостаточно маны.\n";
        return false;
    }

    int px = player.getX();
    int py = player.getY();

    // Проверяем, есть ли место, куда можно "бросить" взрыв
    for (int y = std::max(0, py - range); y <= std::min(field.getHeight() - 1, py + range); ++y) {
        for (int x = std::max(0, px - range); x <= std::min(field.getWidth() - 1, px + range); ++x) {
            // хотя бы одно пустое место в радиусе — значит можно кастовать
            if (field.getCellType(x, y) != CellType::Wall)
                return true;
        }
    }

    std::cout << "Невозможно применить заклинание — все клетки заблокированы.\n";
    return false;
}

void ExplosionSpell::cast(Field& field, Player& player) {
    if (player.getMana() < manaCost) {
        std::cout << "Недостаточно маны!\n";
        return;
    }

    int px = player.getX();
    int py = player.getY();

    std::vector<std::pair<int, int>> available;

    // Собираем доступные точки для применения (в радиусе)
    for (int y = std::max(0, py - range); y <= std::min(field.getHeight() - 1, py + range); ++y) {
        for (int x = std::max(0, px - range); x <= std::min(field.getWidth() - 1, px + range); ++x) {
            if (field.getCellType(x, y) != CellType::Wall)
                available.push_back({x, y});
        }
    }

    if (available.empty()) {
        std::cout << "Нет подходящего места для взрыва.\n";
        return;
    }

    


    // Выбор центра взрыва
    std::cout << "Выберите точку для взрыва (центр области "<<areaSize*2+1<<"x"<<areaSize*2+1<<"):\n";
    for (size_t i = 0; i < available.size(); ++i)
        std::cout << i + 1 << ") (" << available[i].first << ", " << available[i].second << ")\n";

    
    

    int choice;
    int choice_flag = 1;
    while (choice_flag) {
        std::cout << "Введите номер точки: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число!\n";
            continue;
        }
        if (choice >= 1 && choice <= (int)available.size()){
            choice_flag = 0;
        } else {
            std::cout << "Некорректный номер. Попробуйте снова.\n";
        }
        
    }

    auto [cx, cy] = available[choice - 1];

    std::cout << "Взрыв радиусом " << areaSize
              << " вокруг точки (" << cx << ", " << cy << ")!\n";
    for (int y = cy - areaSize; y <= cy + areaSize; ++y) {
        for (int x = cx - areaSize; x <= cx + areaSize; ++x) {
            // Проверка на выход за пределы поля
            if (x < 0 || y < 0 || x >= field.getWidth() || y >= field.getHeight())
                continue;

            GameEnt* ent = field.getCellEntity(x, y);
            if (ent && ent->getObjType() == GameEntType::Enemy) {
                ent->takeDamage(damage);
                std::cout << "Враг (" << x << ", " << y << ") получил " 
                          << damage << " урона.\n";
                if (!ent->isAlive()) {
                    field.clearCellEnt(x, y);
                }
            } else if (ent && ent->getObjType() == GameEntType::Tower) {
                ent->takeDamage(damage);
                std::cout << "Башня (" << x << ", " << y << ") получила " 
                          << damage << " урона.\n";
                if (!ent->isAlive()) {
                    field.clearCellEnt(x, y);
                }
            }
        }
    }

    player.reduceMana(manaCost);
    std::cout << "Заклинание использовано. Потрачено " << manaCost << " маны.\n";
}
