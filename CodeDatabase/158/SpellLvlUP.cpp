#include "SpellLvlUP.h"

void SpellLvlUP::displayUpgrades() 
{
    auto spells = player.listSpells();
    std::cout << "Spell Level-Up Shop:\n";
    for(size_t i = 0; i < spells.size(); i++) 
    {
        SpellHand spell(static_cast<SpellType>(spells[i].first));
        std::cout << i+1 << ". " << spell.getName() << " (LvL " << spells[i].second 
                  << ") - " << spell.getDescription() << "\n";
    }
}

bool SpellLvlUP::upgradeSpell(int choice) 
{
    auto spells = player.listSpells();
    if(choice < 1 || choice > static_cast<int>(spells.size())) return false;
    SpellHand spell(static_cast<SpellType>(spells[choice-1].first));
    if(player.getSouls() >= spell.getPrice()) 
    {
        player.spendSoul(spell.getPrice());
        player.upgradeSpell(spells[choice-1].first);
        return true;
    }
    return false;
}