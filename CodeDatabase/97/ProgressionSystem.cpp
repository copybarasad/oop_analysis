#include "ProgressionSystem.h"
#include "GameState.h"
#include "Player.h"
#include <iostream>
#include <sstream>

void ProgressionSystem::applyLevelUp(GameState &state) {
    auto pl = state.player();
    if (!pl) return;
    std::cout << "Choose a level-up reward:\n";
    std::cout << "1) +5 Max HP (and fully restore)\n";
    std::cout << "2) +2 Near Damage\n";
    std::cout << "3) +1 Far Damage\n";
    std::cout << "4) +1 Hand Capacity\n";
    int choice = 0;
    while (true) {
        std::cout << "Enter choice (1-4): ";
        std::string line; if (!std::getline(std::cin, line)) return;
        std::istringstream ss(line);
        if (ss >> choice && choice >=1 && choice <=4) break;
    }
    switch (choice) {
        case 1: pl->increaseHpMax(5); std::cout << "Max HP increased to " << pl->hpMax() << "\n"; break;
        case 2: pl->increaseNearDamage(2); std::cout << "Near damage increased to " << pl->getNearDamage() << "\n"; break;
        case 3: pl->increaseFarDamage(1); std::cout << "Far damage increased to " << pl->getFarDamage() << "\n"; break;
        case 4: pl->increaseHandCapacity(1); std::cout << "Hand capacity increased to " << pl->hand().capacity() << "\n"; break;
    }
}

void ProgressionSystem::scaleEnemyStats(int level, int &outHp, int &outDmg) const {
    outHp = 6 + (level - 1) * 2;
    outDmg = 2 + (level - 1) / 2;
}
