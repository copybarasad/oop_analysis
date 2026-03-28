#include "SpellManager.h"

SpellManager::SpellManager() : gen(rd()) {}

void SpellManager::upgradeAllSpells(Player& player) {
    auto& hand = player.getPlayerHand();
    bool upgradedAny = false;

    for (int slotIndex = 0; slotIndex < hand.getMaxCapacity(); ++slotIndex) {
        for (int spellIndex = 0; spellIndex < hand.getSpellNum(slotIndex); ++spellIndex) {
            auto spell = hand.getSpell(slotIndex);
            if (spell) {
                spell->upgradeCard(1);
                upgradedAny = true;
            }
        }
    }
    
    if (upgradedAny) {
        std::cout << "Все заклинания усилены на 1 уровень!" << std::endl;
    } else {
        std::cout << "У вас нет заклинаний для усиления!" << std::endl;
    }
}

void SpellManager::removeHalfSpells(Player& player) {
    auto& hand = player.getPlayerHand();
    int currentSize = hand.getCurrentSize();
    
    if (currentSize > 1) {
        int spellsToRemove = currentSize / 2;
        
        for (int i = 0; i < spellsToRemove; ++i) {
            if (hand.getCurrentSize() > 1) {
                std::uniform_int_distribution<> slotDis(0, hand.getMaxCapacity() - 1);
                int slotIndex = slotDis(gen);

                if (hand.getSpellNum(slotIndex) > 0) {
                    hand.removeSpell(slotIndex);
                } else {
                    i--;
                }
            }
        }
        std::cout << "Удалено " << spellsToRemove << " заклинаний." << std::endl;
    }
}

void SpellManager::addRandomSpell(Player& player) {
    std::uniform_int_distribution<> dis(0, 4);
    int spellType = dis(gen);

    addSpecificSpell(player, spellType);
}

void SpellManager::addSpecificSpell(Player& player, int spellType, int level) {
    auto& hand = player.getPlayerHand();
    
    switch (spellType) {
        case 0:
            hand.addSpell(std::make_unique<DirectFightSpell>(level), 0);
            break;
        case 1:
            hand.addSpell(std::make_unique<AreaDamageSpell>(level), 1);
            break;
        case 2:
            hand.addSpell(std::make_unique<TrapSpell>(level), 2);
            break;
        case 3:
            hand.addSpell(std::make_unique<SummonSpell>(level), 3);
            break;
        case 4:
            hand.addSpell(std::make_unique<EnhancingSpell>(level), 4);
            break;
        default:
            break;
    }
    
    std::cout << "Получено новое заклинание!" << std::endl;
}