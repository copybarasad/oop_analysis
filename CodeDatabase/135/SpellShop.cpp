#include "SpellShop.h"
#include "GameField.h"
#include "EntityManager.h"
#include "Player.h"
#include "SpellFactory.h"
#include "SpellHand.h"
#include "PlayerResources.h"
#include "ISpell.h"
#include "GameConstants.h"
#include <iostream>
#include <limits>

SpellShop::SpellShop(GameField& game_field, EntityManager& em, int& player_idx, SpellFactory& factory)
	: field(game_field), entity_manager(em), player_index(player_idx), spell_factory(factory) {}

Player* SpellShop::getPlayer() {
	auto* entity = entity_manager.getEntity(player_index);
	if (!entity) return nullptr;
	return dynamic_cast<Player*>(entity);
}

void SpellShop::displayShopHeader(int current_spells, int max_spells, int points) {
	std::cout << "\n╔════════════════════════════════════════╗\n";
	std::cout << "║         МАГАЗИН ЗАКЛИНАНИЙ            ║\n";
	std::cout << "╚════════════════════════════════════════╝\n";
	std::cout << "→ Заклинаний в руке: " << current_spells << "/" << max_spells << "\n";
	std::cout << "→ Очков: " << points << "\n";
}

bool SpellShop::validatePurchaseConditions(int current_spells, int max_spells, int points) {
	if (current_spells >= static_cast<int>(max_spells)) {
		std::cout << "✗ Рука полна! Используйте заклинания, чтобы освободить место.\n";
		return false;
	}
	
	if (points < GameConstants::SPELL_COST) {
		std::cout << "✗ Недостаточно очков! Нужно: " << GameConstants::SPELL_COST 
		          << ", у вас: " << points << "\n";
		return false;
	}
	
	return true;
}

void SpellShop::displaySpellMenu() {
	std::cout << "\nСтоимость заклинания: " << GameConstants::SPELL_COST << " очков\n";
	std::cout << "\nДоступные заклинания:\n";
	std::cout << "  1. Прямой урон (15 урона, радиус 5)\n";
	std::cout << "  2. Урон по площади (10 урона, радиус 4, площадь 2x2)\n";
	std::cout << "  3. Ловушка (20 урона)\n";
	std::cout << "  4. Призыв союзника (HP 25, урон 5)\n";
	std::cout << "  5. Усиление (увеличивает эффект следующего заклинания)\n";
	std::cout << "  6. Случайное заклинание\n";
	std::cout << "  q. Закрыть магазин\n";
}

char SpellShop::getUserChoice() {
	std::cout << "\nВыберите заклинание: ";
	char choice;
	if (!(std::cin >> choice)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return '\0';
	}
	return choice;
}

std::unique_ptr<ISpell> SpellShop::createSpellByChoice(char choice) {
	switch (choice) {
		case '1':
			std::cout << "✓ Куплено: Заклинание прямого урона!\n";
			return spell_factory.createDirectDamage(15, 5);
		case '2':
			std::cout << "✓ Куплено: Заклинание урона по площади!\n";
			return spell_factory.createAreaDamage(10, 4, 2);
		case '3':
			std::cout << "✓ Куплена: Ловушка!\n";
			return spell_factory.createTrap(20, 5);
		case '4':
			std::cout << "✓ Куплено: Заклинание призыва!\n";
			return spell_factory.createSummon(1, 25, 5);
		case '5':
			std::cout << "✓ Куплено: Заклинание усиления!\n";
			return spell_factory.createEnhancement();
		case '6':
			std::cout << "✓ Куплено: Случайное заклинание!\n";
			return spell_factory.createRandomSpell();
		default:
			std::cout << "✗ Неверный выбор!\n";
			return nullptr;
	}
}

void SpellShop::processPurchase(std::unique_ptr<ISpell> spell, Player& player) {
	if (!spell) return;
	
	SpellHand& spell_hand = player.getSpellHand();
	PlayerResources& resources = player.getResources();
	
	if (spell_hand.addSpell(std::move(spell))) {
		if (resources.spendPoints(GameConstants::SPELL_COST)) {
			std::cout << "→ Заклинание добавлено в руку.\n";
			std::cout << "→ Списано очков: " << GameConstants::SPELL_COST << "\n";
			std::cout << "→ Осталось очков: " << resources.getPoints() << "\n";
			std::cout << "→ Теперь в руке: " << spell_hand.getCurrentSize() 
			          << "/" << spell_hand.getMaxSize() << " заклинаний.\n";
		} else {
			std::cout << "✗ Ошибка при списании очков!\n";
		}
	} else {
		std::cout << "✗ Не удалось добавить заклинание в руку!\n";
	}
}

void SpellShop::openShop() {
	Player* player = getPlayer();
	if (!player) return;
	
	SpellHand& spell_hand = player->getSpellHand();
	PlayerResources& resources = player->getResources();
	
	displayShopHeader(spell_hand.getCurrentSize(), spell_hand.getMaxSize(), resources.getPoints());
	
	if (!validatePurchaseConditions(spell_hand.getCurrentSize(), spell_hand.getMaxSize(), 
	                                resources.getPoints())) {
		return;
	}
	
	displaySpellMenu();
	
	char choice = getUserChoice();
	if (choice == '\0') {
		std::cout << "✗ Ошибка ввода!\n";
		return;
	}
	
	if (choice == 'q') {
		std::cout << "Магазин закрыт.\n";
		return;
	}
	
	auto spell = createSpellByChoice(choice);
	processPurchase(std::move(spell), *player);
}
