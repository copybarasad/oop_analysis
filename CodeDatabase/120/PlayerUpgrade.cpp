#include "PlayerUpgrade.h"
#ifdef _WIN64
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

PlayerUpgrade::PlayerUpgrade(Player& p, GameLogic& log)
    : player(p), logic(log) {}

void PlayerUpgrade::ShowUpgradeMenu() {
  while (true) {
    if (system(CLEAR_COMMAND) != 0) {
    }

    std::cout << "=== Upgrade Menu ===\n";
    std::cout << "Player Points: " << player.GetScore() << "\n";
    std::cout << "1. Increase Health (+20)\n";
    std::cout << "2. Increase Damage (+2)\n";
    std::cout << "3. Increase Hand Size (+1)\n";
    std::cout << "4. Finish Upgrades\n";
    std::cout << "Choose option: ";

    int choice;
    std::cin >> choice;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }

    const int cost = 10;
    if (choice >= 1 && choice <= 3 && player.GetScore() < cost) {
      std::cout << "Not enough points! Each upgrade costs " << cost
                << " points.\n";
      std::cin.get();
      continue;
    }

    switch (choice) {
      case 1:
        player.AddScore(-cost);
        player.IncreaseMaxHealth(20);
        break;
      case 2:
        player.AddScore(-cost);
        player.IncreaseDamage(2);
        break;
      case 3:
        player.AddScore(-cost);
        player.IncreaseHandSize(1);
        break;
      case 4:
        return;
      default:
        continue;
    }
  }
}
