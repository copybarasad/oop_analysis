#include "SpellHand.h"

SpellHand::SpellHand(SpellType type, bool active) : type(type), active(active), LvL(1) 
{
    switch(type) 
    {
        case SpellType::DEATH_NOTE:
            name = "Death Note";
            description = "Range is when you can see their face and name";
            upgrade_desc = "Eyes of Gods of Death";
            price = 5;
            damage = 999;
            range = 3;
            AoE = 0;
            enemies_required = true;
            cooldown = 0;
            max_amount = 1;
            break;
        case SpellType::PSI_STORM:
            name = "Explosion";
            description = "2x2 cells AoE - their probes are under attack!";
            upgrade_desc = "Larger AoE";
            price = 4;
            damage = 2;
            range = 6;
            AoE = 2;
            enemies_required = false;
            cooldown = 0;
            max_amount = 2;
            break;
        case SpellType::TRAP:
            name = "Trap";
            description = "Set trap on the field; Makes BOOM when is stepped on";
            upgrade_desc = "More damage";
            price = 3;
            damage = 3;
            range = 6;
            AoE = 0;
            enemies_required = false;
            cooldown = 0;
            max_amount = 3;
            break;
        case SpellType::DOPPELGANGER:
            name = "Doppelganger";
            description = "Pop in two for you";
            upgrade_desc = "More copies";
            price = 6;
            damage = 1;
            range = 6;
            AoE = 0;
            enemies_required = false;
            cooldown = 0;
            max_amount = 2;
            break;
        case SpellType::LEVEL_UP:
            name = "Spell Level-Up";
            description = "Upgrade existing spells";
            upgrade_desc = "Accumulate level-ups";
            price = 8;
            damage = 0;
            range = 0;
            AoE = 0;
            enemies_required = false;
            cooldown = 0;
            max_amount = 5;
            break;
    }
}

void SpellHand::Upgrade() 
{
    LvL++;
    switch(type) 
    {
        case SpellType::DEATH_NOTE: 
            range += 1; 
            std::cout << "Death Note upgraded! Range now: " << range << "\n";
            break;
        case SpellType::PSI_STORM: 
            AoE += 1; 
            damage += 1;
            std::cout << "Explosion upgraded! AoE: " << AoE << "x" << AoE 
                      << ", Damage: " << damage << "\n";
            break;
        case SpellType::TRAP: 
            damage += 1; 
            std::cout << "Trap upgraded! Damage: " << damage << "\n";
            break;
        case SpellType::DOPPELGANGER: 
            max_amount += 1; 
            std::cout << "Doppelganger upgraded! Max copies: " << max_amount << "\n";
            break;
        case SpellType::LEVEL_UP: 
            std::cout << "Level-up capacity increased!\n";
            break;
    }
}

SpellType SpellHand::getType() const { return type; }
std::string SpellHand::getName() const { return name; }
std::string SpellHand::getDescription() const { return description; }
int SpellHand::getPrice() const { return price; }
int SpellHand::getLvL() const { return LvL; }
int SpellHand::getRange() const { return range; }
int SpellHand::getAoE() const { return AoE; }
bool SpellHand::requiresEnemy() const { return enemies_required; }
int SpellHand::getDamage() const { return damage; }
int SpellHand::getMaxAmount() const { return max_amount; }