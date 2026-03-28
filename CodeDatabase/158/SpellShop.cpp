#include "SpellShop.h"

void SpellShop::displayShop() 
{
    std::cout << "(P) Pyatyorochka - buy a spell\n";
    std::cout << "Available spells:\n";
    
    auto playerSpells = player.listSpells();
    
    SpellHand spells[] = {SpellType::DEATH_NOTE, SpellType::PSI_STORM, SpellType::TRAP, 
                         SpellType::DOPPELGANGER, SpellType::LEVEL_UP};
    
    for(int i = 0; i < 5; i++) 
    {
        int currentLevel = 1;
        for(const auto& spell : playerSpells) 
        {
            if(spell.first == i) 
            {
                currentLevel = spell.second;
                break;
            }
        }
        
        SpellHand spell(static_cast<SpellType>(i));
        std::cout << i+1 << ". " << spell.getName() << " (LvL " << currentLevel << ") - " 
                  << spell.getDescription() << " - Price: " << spell.getPrice() << " Souls\n";
    }
}

bool SpellShop::buySpell(int choice) 
{
    SpellType type = static_cast<SpellType>(choice - 1);
    SpellHand spell(type);
    if(player.getSouls() >= spell.getPrice() && player.listSpells().size() < player.getMaxSpells()) 
    {
        player.spendSoul(spell.getPrice());
        player.addSpell(static_cast<int>(type), 1);
        return true;
    } 
    else if (player.listSpells().size() >= player.getMaxSpells())
        std::cout << "Spell hand is full! Max spells: " << player.getMaxSpells() << "\n";
    return false;
}