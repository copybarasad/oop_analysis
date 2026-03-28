#ifndef SPELL_SHOP_H
#define SPELL_SHOP_H

#include <memory>

class GameField;
class EntityManager;
class SpellFactory;
class Player;
class ISpell;

class SpellShop {
private:
	GameField& field;
	EntityManager& entity_manager;
	int& player_index;
	SpellFactory& spell_factory;
	
	Player* getPlayer();
	void displayShopHeader(int current_spells, int max_spells, int points);
	bool validatePurchaseConditions(int current_spells, int max_spells, int points);
	void displaySpellMenu();
	char getUserChoice();
	std::unique_ptr<ISpell> createSpellByChoice(char choice);
	void processPurchase(std::unique_ptr<ISpell> spell, Player& player);

public:
	SpellShop(GameField& game_field, EntityManager& em, int& player_idx, SpellFactory& factory);
	void openShop();
};

#endif
