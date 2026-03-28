#include "spellshop.h"
#include "spellcard.h"  
#include <iostream>
#include <memory>

SpellShop::SpellShop() { 
    availableSpells = { 
        std::make_shared<MagicArrowSpell>(),
        std::make_shared<FireballSpell>(),
        std::make_shared<IceStormSpell>(),
        std::make_shared<MagicTrapSpell>()
    };
}


void SpellShop::displayShop() const {
    std::cout << "\n=== МАГАЗИН ЗАКЛИНАНИЙ ===\n";
    std::cout << "Стоимость: 100 очков за заклинание\n\n";
    
    for (size_t i = 0; i < availableSpells.size(); ++i) {
        auto spell = availableSpells[i];
        std::cout << (i + 1) << ". " << spell->getName() << "\n";
        std::cout << "   " << spell->getDescription() << "\n";
        std::cout << "   Мана: " << spell->getManaCost();
        std::cout << ", Урон/Эффект: " << spell->getValue();
        std::cout << ", Радиус: " << spell->getRadius() << "\n\n";
    }
    
    std::cout << "0. Выйти из магазина\n\n";
}

std::shared_ptr<SpellCard> SpellShop::buySpell(int index) {
    if (index < 0 || index >= availableSpells.size()) {
        throw std::out_of_range("Неверный индекс заклинания");
    }
    return availableSpells[index];
}


int SpellShop::getSpellCount() const { 
    return availableSpells.size(); 
}