#include "SpellShop.h"
#include <iostream>
#include <random>
#include <limits>

SpellShop::SpellShop(SpellHand& spellHand) 
    : playerSpellHand(&spellHand) {
    initializeSpells();
}

void SpellShop::openShop() {
    if (!playerSpellHand) return;
    
    std::cout << "\n=== WELCOME TO THE SPELL SHOP ===\n";
    std::cout << "Your mana: " << playerSpellHand->getMana() << "\n";
    std::cout << "Spell slots: " << playerSpellHand->getSpellCount() 
              << "/" << playerSpellHand->getMaxSize() << "\n\n";
    
    displayShop();
    
    if (playerSpellHand->isFull()) {
        std::cout << "Your spell hand is full! Cannot buy more spells.\n";
        return;
    }
    
    std::cout << "Enter spell number to purchase (0 to exit): ";
    int choice;
    std::cin >> choice;
    
    if (choice > 0 && choice <= static_cast<int>(availableSpells.size())) {
        purchaseSpell(choice - 1);
    } else if (choice != 0) {
        std::cout << "Invalid choice!\n";
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void SpellShop::giveRandomStarterSpell() {
    if (!playerSpellHand || availableSpells.empty() || playerSpellHand->isFull()) {
        return;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, availableSpells.size() - 1);
    
    int randomIndex = dis(gen);
    
    // Создаем копию заклинания для передачи
    std::unique_ptr<Spell> spellCopy;
    const Spell* originalSpell = availableSpells[randomIndex].get();
    
    // Создаем копию в зависимости от типа заклинания
    if (dynamic_cast<const DirectDamageSpell*>(originalSpell)) {
        spellCopy = std::make_unique<DirectDamageSpell>(
            originalSpell->getName(), 
            25,  // damage
            30,  // manaCost  
            5    // range
        );
    } else if (dynamic_cast<const AreaDamageSpell*>(originalSpell)) {
        spellCopy = std::make_unique<AreaDamageSpell>(
            originalSpell->getName(),
            20,  // damage
            40,  // manaCost
            5    // range
        );
    } else if (dynamic_cast<const TrapSpell*>(originalSpell)) {
        spellCopy = std::make_unique<TrapSpell>(
            originalSpell->getName(),
            30,  // damage
            35,  // manaCost
            999  // range (full map)
        );
    }
    
    if (spellCopy) {
        playerSpellHand->addSpell(std::move(spellCopy));
        std::cout << "You received a starter spell: " << originalSpell->getName() << "\n";
    }
}

void SpellShop::initializeSpells() {
    availableSpells.clear();
    
    availableSpells.push_back(std::make_unique<DirectDamageSpell>(
        "Fire Bolt", 25, 30, 5
    ));
    
    availableSpells.push_back(std::make_unique<AreaDamageSpell>(
        "Ice Nova", 20, 40, 5
    ));
    
    availableSpells.push_back(std::make_unique<TrapSpell>(
        "Spike Trap", 30, 35, 999
    ));
    
    availableSpells.push_back(std::make_unique<DirectDamageSpell>(
        "Lightning Strike", 35, 50, 6
    ));
}

void SpellShop::displayShop() const {
    std::cout << "Available Spells:\n";
    for (size_t i = 0; i < availableSpells.size(); ++i) {
        const Spell* spell = availableSpells[i].get();
        std::cout << i + 1 << ". " << spell->getName() 
                  << " - " << spell->getDescription()
                  << " [Cost: " << spell->getManaCost() 
                  << ", Range: " << spell->getRange() << "]\n";
    }
    std::cout << "\n";
}

bool SpellShop::purchaseSpell(int spellIndex) {
    if (!playerSpellHand || spellIndex < 0 || spellIndex >= static_cast<int>(availableSpells.size())) {
        std::cout << "Invalid spell selection!\n";
        return false;
    }
    
    const Spell* selectedSpell = availableSpells[spellIndex].get();
    int cost = selectedSpell->getManaCost();
    
    if (playerSpellHand->getMana() < cost) {
        std::cout << "Not enough mana! Need " << cost << " but have " 
                  << playerSpellHand->getMana() << ".\n";
        return false;
    }
    
    if (playerSpellHand->isFull()) {
        std::cout << "Spell hand is full! Cannot buy more spells.\n";
        return false;
    }
    
    // Создаем копию заклинания для покупки
    std::unique_ptr<Spell> spellCopy;
    
    if (dynamic_cast<const DirectDamageSpell*>(selectedSpell)) {
        spellCopy = std::make_unique<DirectDamageSpell>(
            selectedSpell->getName(), 
            25, cost, 5
        );
    } else if (dynamic_cast<const AreaDamageSpell*>(selectedSpell)) {
        spellCopy = std::make_unique<AreaDamageSpell>(
            selectedSpell->getName(),
            20, cost, 5
        );
    } else if (dynamic_cast<const TrapSpell*>(selectedSpell)) {
        spellCopy = std::make_unique<TrapSpell>(
            selectedSpell->getName(),
            30, cost, 999
        );
    }
    
    if (spellCopy && playerSpellHand->spendMana(cost)) {
        if (playerSpellHand->addSpell(std::move(spellCopy))) {
            std::cout << "Purchased " << selectedSpell->getName() << " for " << cost << " mana!\n";
            return true;
        }
    }
    
    std::cout << "Failed to purchase spell!\n";
    return false;
}