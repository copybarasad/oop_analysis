#include "game_utils.h"
#include <iostream>
#include <iomanip>

void GameStatePrinter::printGameState(const GameController& game) const {
	const Player& player = game.getPlayer();
	
	std::cout << "\n" << std::string(50, '=') << std::endl;
	std::cout << "=== LEVEL " << game.getCurrentLevel() << " ===" << std::endl;
	std::cout << std::string(50, '=') << std::endl;
	
	// Player information
	std::cout << "PLAYER:" << std::endl;
	std::cout << "  Health: " << player.getHealth() << " HP" << std::endl;
	std::cout << "  Mana: " << player.getMana() << "/" << player.getMaxMana() << " MP" << std::endl;
	std::cout << "  Mode: " << (player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED") << std::endl;
	std::cout << "  Score: " << player.getScore() << " points" << std::endl;

	// Spells
	std::cout << "\nSPELLS:" << std::endl;
	if (player.getSpellHand().getSpellCount() > 0) {
		for (int i = 0; i < player.getSpellHand().getSpellCount(); ++i) {
			SpellCard* spell = player.getSpellHand().getSpell(i);
			if (spell) {
				std::cout << "  " << i << ". " << spell->getName() << " (Cost: " << spell->getManaCost() << " MP)";
				if (auto* directSpell = dynamic_cast<DirectDamageSpell*>(spell)) {
					std::cout << " - Damage: " << directSpell->getDamage();
				} else if (auto* areaSpell = dynamic_cast<AreaDamageSpell*>(spell)) {
					std::cout << " - Area: " << areaSpell->getAreaSize() << "x" << areaSpell->getAreaSize();
				} else if (dynamic_cast<TrapSpell*>(spell)) {
					std::cout << " - Trap";
				}
				std::cout << std::endl;
			}
		}
	} else {
		std::cout << "  No spells available" << std::endl;
	}
	
	// Enemies and objects
	std::cout << "\nENEMIES & OBJECTS:" << std::endl;
	std::cout << "  Enemies: " << game.getEnemies().size() << std::endl;
	std::cout << "  Buildings: " << game.getBuildings().size() << std::endl;
	std::cout << "  Towers: " << game.getTowers().size() << std::endl;
	
	// Active traps
	const auto& traps = game.getActiveTraps();
	if (!traps.empty()) {
		std::cout << "  Active Traps: " << traps.size() << std::endl;
	}
	
	// Commands
	std::cout << "\nCOMMANDS:" << std::endl;
	std::cout << "  Movement: W,A,S,D" << std::endl;
	std::cout << "  Switch Mode: M" << std::endl;
	std::cout << "  Cast Spell: C" << std::endl;
	std::cout << "  Buy Spell: B" << std::endl;
	std::cout << "  Game Menu: P" << std::endl;
	std::cout << "  Quit: Q" << std::endl;
	
	std::cout << std::string(50, '=') << std::endl;
}