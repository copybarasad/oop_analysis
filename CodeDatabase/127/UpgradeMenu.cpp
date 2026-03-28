// UpgradeMenu.cpp

#include "UpgradeMenu.h"
#include "Player.h"
#include <iostream>
#include <string>

void UpgradeMenu::present(Player& player) {
    std::cout << "=== Меню апгрейдов ===\n";
    std::cout << "Выберите апгрейд (введите номер):\n";
    std::cout << "1) +20 к HP (восстанавливает сразу)\n";
    std::cout << "2) +5 к ближнему урону\n";
    std::cout << "3) +5 к дальнему урону\n";
    std::cout << "4) +1 к дальнему радиусу\n";
    std::cout << "5) +1 к следующему усилению (extraDamage)\n";
    std::cout << "0) Пропустить\n";
    std::cout << "Ваш выбор: ";

    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) std::getline(std::cin, line);
    int choice = 0;
    try { choice = std::stoi(line); }
    catch (...) { choice = -1; }

    switch (choice) {
    case 1:
        player.addHP(10);
        std::cout << "HP увеличено на 10.\n";
        break;
    case 2:
        player.increaseMeleeDamage(5);
        std::cout << "Ближний урон увеличен на 5.\n";
        break;
    case 3:
        player.increaseRangedDamage(5);
        std::cout << "Дальний урон увеличен на 5.\n";
        break;
    case 4:
        player.increaseRangedRange(1);
        std::cout << "Радиус дальнего боя увеличен на 1.\n";
        break;
    case 5: {
        SpellEnhancement enh;
        enh.extraDamage = 1;
        player.addPendingEnhancement(enh);
        std::cout << "Добавлено +1 к следующему усилению урона.\n";
        break;
    }
    default:
        std::cout << "Пропуск апгрейда.\n";
        break;
    }
    std::cout << "======================\n";
}
