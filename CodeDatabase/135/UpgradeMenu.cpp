#include "UpgradeMenu.h"
#include <iostream>
#include <limits>

void UpgradeMenu::clearInputBuffer() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void UpgradeMenu::displayPlayerStats(const Player& player) {
	auto health = player.getHealth();
	std::cout << "\n╔═══════════════════════════════════════╗\n";
	std::cout << "║       ТЕКУЩИЕ ХАРАКТЕРИСТИКИ          ║\n";
	std::cout << "╚═══════════════════════════════════════╝\n";
	std::cout << "Здоровье: " << health.first << "/" << health.second << "\n";
	std::cout << "Интеллект: " << player.getInt() << "\n";
	std::cout << "Ловкость: " << player.getDex() << "\n";
	std::cout << "Сила: " << player.getStr() << "\n";
	std::cout << "Размер руки заклинаний: " << player.getSpellHand().getMaxSize() << "\n";
}

void UpgradeMenu::displayUpgradeOptions(const Player& player) {
	displayPlayerStats(player);
	
	std::cout << "\n╔═══════════════════════════════════════╗\n";
	std::cout << "║   ПОВЫШЕНИЕ УРОВНЯ! Выберите улучшение: ║\n";
	std::cout << "╚═══════════════════════════════════════╝\n";
	std::cout << "\n1. Увеличить Интеллект (+3)\n";
	std::cout << "2. Увеличить Ловкость (+3)\n";
	std::cout << "3. Увеличить Силу (+3)\n";
	std::cout << "4. Увеличить Макс. Здоровье (+20)\n";
	std::cout << "5. Увеличить размер руки (+1)\n";
	std::cout << "6. Добавить усиление заклинаний\n";
	std::cout << "\nВаш выбор: ";
}

UpgradeType UpgradeMenu::selectUpgrade() {
	char choice;
	std::cin >> choice;
	clearInputBuffer();
	
	switch (choice) {
		case '1':
			return UpgradeType::INTELLIGENCE;
		case '2':
			return UpgradeType::DEXTERITY;
		case '3':
			return UpgradeType::STRENGTH;
		case '4':
			return UpgradeType::MAX_HEALTH;
		case '5':
			return UpgradeType::HAND_SIZE;
		case '6':
			return UpgradeType::SPELL_ENHANCEMENT;
		default:
			std::cout << "\nНеверный выбор. По умолчанию выбран интеллект.\n";
			return UpgradeType::INTELLIGENCE;
	}
}

void UpgradeMenu::showUpgradeScreen(Player& player, UpgradeSystem& upgrade_system) {
	displayUpgradeOptions(player);
	UpgradeType chosen_upgrade = selectUpgrade();
	upgrade_system.applyUpgrade(player, chosen_upgrade);
	
	std::cout << "\n✓ Улучшение успешно применено!\n";
	std::cout << "\nНажмите Enter для продолжения...\n";
	std::cin.get();
}
