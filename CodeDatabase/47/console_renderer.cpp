#include "console_renderer.h"
#include "game.h"
#include <iostream>

void ConsoleRenderer::render(Game& game) {
    game.drawFieldPublic();
    
    std::cout << "\n";
    std::cout << "Player position: (" << game.getPlayerPositionX() << ", " << game.getPlayerPositionY() << ")\n";
    
    Player& player = game.getPlayer();
    std::cout << "Health: " << player.getHealth() << "/" << player.getMaxHealth() << "\n";
    std::cout << "Mana: " << player.getSpellHand().getCurrentMana() << "/" << player.getSpellHand().getMaxMana() << "\n";
    std::cout << "Combat Mode: " << (player.getCombatMode() == CombatMode::MELEE ? "Melee" : "Ranged") << "\n";
    std::cout << "Damage: " << player.getDamage() << " (Melee: " << player.getMeleeDamage() << ", Ranged: " << player.getRangedDamage() << ")\n";
    std::cout << "Level: " << game.getCurrentLevel() << "\n";
    
    std::cout << "Spells:" << std::endl;
    for (int i = 0; i < player.getSpellHand().getSpellCount(); ++i) {
        auto spell = player.getSpellHand().getSpells()[i].get();
        std::cout << (i+1) << "." << spell->getName() << " (Mana: " << spell->getManaCost() << ", Range: " << spell->getRange() << ", Damage: " << spell->getDamage() << ")" << std::endl;
    }
}
