#include "InputHandler.h"
#include <iostream>
#include <cmath>

InputHandler::InputHandler(Wanderer& wanderer, GameField& field, Position& wandererPos,
                         CombatManager& combatManager, BeastManager& beastManager, bool& canMove, bool& onSlowCell, bool& running)
    : wanderer(wanderer), field(field), wandererPosition(wandererPos),
      combatManager(combatManager), beastManager(beastManager), wandererCanMove(canMove), 
      wandererOnSlowCell(onSlowCell), gameRunning(running),
      selectingSpellTarget(false), selectedSpellIndex(-1) {}

void InputHandler::updateAllManagersPosition(const Position& newPos) {
    wandererPosition = newPos;
    beastManager.updateWandererPosition(newPos);
    combatManager.updateWandererPosition(newPos);
}

void InputHandler::processInput(char input) {
    if (selectingSpellTarget) {
        processSpellTargetSelection(input);
        return;
    }
    
    if (!wandererCanMove || !wanderer.getIsAlive()) {
        if (!wanderer.getIsAlive()) {
            gameRunning = false;
            return;
        }
        std::cout << "You are slowed and cannot move this turn!\n";
        wandererCanMove = true;
        return;
    }
    
    int newX = wandererPosition.x;
    int newY = wandererPosition.y;
    bool moved = false;
    
    switch (input) {
        case 'w': case 'W': newY--; moved = true; break;
        case 's': case 'S': newY++; moved = true; break;
        case 'a': case 'A': newX--; moved = true; break;
        case 'd': case 'D': newX++; moved = true; break;
        case 'c': case 'C': 
            wanderer.switchCombatMode();
            std::cout << "Switched to " << 
                (wanderer.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED") << " combat\n";
            return;
        case 'b': case 'B':
            processSpellPurchase();
            return;
        case 'q': case 'Q':
            gameRunning = false;
            std::cout << "Game ended by wanderer.\n";
            return;
    }
    
    if (input >= '1' && input <= '3') {
        processSpellSelection(input);
        return;
    }
    
    if (moved) {
        CombatMode mode = wanderer.getCombatMode();
        
        bool hasBeast = field.isValidPosition(newX, newY) && field.getCell(newX, newY).getHasBeast();
        bool hasBuilding = field.isValidPosition(newX, newY) && field.getCell(newX, newY).getHasBuilding();

        if (mode == CombatMode::MELEE && (hasBeast || hasBuilding)) {
            combatManager.processMeleeAttack(Position(newX, newY));
            if (wanderer.getIsAlive()) {
                field.getCell(wandererPosition.x, wandererPosition.y).setHasWanderer(true);
            }
            return;
        } else if (mode == CombatMode::RANGED && !hasBeast && !hasBuilding) {
            int directionX = newX - wandererPosition.x;
            int directionY = newY - wandererPosition.y;
            
            combatManager.processRangedAttack(directionX, directionY);
            if (wanderer.getIsAlive()) {
                field.getCell(wandererPosition.x, wandererPosition.y).setHasWanderer(true);
            }
            return;
        }
        
        if (field.moveEntity(wandererPosition.x, wandererPosition.y, newX, newY, true)) {
            Cell& newCell = field.getCell(newX, newY);
            if (newCell.getHasTrap()) {
                wanderer.takeDamage(newCell.getTrapDamage());
                std::cout << "You triggered a trap for " << newCell.getTrapDamage() << " damage!\n";
                newCell.triggerTrap();
            }
            updateAllManagersPosition(Position(newX, newY));
            applyCellEffects(newX, newY);
        } else {
            std::cout << "Cannot move there!\n";
            if (wanderer.getIsAlive()) {
                field.getCell(wandererPosition.x, wandererPosition.y).setHasWanderer(true);
            }
        }
    }
}

void InputHandler::applyCellEffects(int x, int y) {
    CellType cellType = field.getCell(x, y).getType();
    
    if (cellType == CellType::SLOW && !wandererOnSlowCell) {
        wandererOnSlowCell = true;
        wandererCanMove = false;
        std::cout << "Stepped on slow cell! Next turn skipped.\n";
    } else if (cellType != CellType::SLOW && wandererOnSlowCell) {
        wandererOnSlowCell = false;
    }
}

void InputHandler::processSpellPurchase() {
    Hand& hand = wanderer.getHand();
    if (hand.getSpellCount() >= hand.getMaxSize()) {
        std::cout << "Spell hand is full! Max size: " << hand.getMaxSize() << "\n";
        return;
    }
    
    if (wanderer.getPoints() < 50) {
        std::cout << "Not enough points! Need 50, but have " << wanderer.getPoints() << "\n";
        return;
    }
    
    SpellFactory factory;
    auto newSpell = factory.createRandomSpell();
    if (newSpell) {
        wanderer.addPoints(-50);
        if (hand.addSpell(std::move(newSpell))) {
            std::cout << "Purchased new spell for 50 points!\n";
        } else {
            std::cout << "Failed to add spell to hand!\n";
            wanderer.addPoints(50);
        }
    }
}

void InputHandler::processSpellSelection(char input) {
    int spellIndex = input - '1';
    
    if (spellIndex < wanderer.getHand().getSpellCount()) {
        SpellCard* spell = wanderer.getHand().getSpell(spellIndex);
        if (spell && !spell->isUsed()) {
            if (wanderer.spendMana(spell->getManaCost())) {
                startSpellTargetSelection(spellIndex);
            } else {
                std::cout << "Not enough mana! Need " << spell->getManaCost() 
                          << ", but have " << wanderer.getMana() << "\n";
            }
        } else {
            std::cout << "Spell not available or already used!\n";
        }
    } else {
        std::cout << "No spell in slot " << (spellIndex + 1) << "!\n";
    }
}

void InputHandler::processSpellTargetSelection(char input) {
    if (!selectingSpellTarget) return;
    
    SpellCard* spell = wanderer.getHand().getSpell(selectedSpellIndex);
    if (!spell) {
        cancelSpellTargetSelection();
        return;
    }
    
    int targetX = wandererPosition.x;
    int targetY = wandererPosition.y;
    bool moved = false;
    
    switch (input) {
        case 'w': case 'W': targetY--; moved = true; break;
        case 's': case 'S': targetY++; moved = true; break;
        case 'a': case 'A': targetX--; moved = true; break;
        case 'd': case 'D': targetX++; moved = true; break;
        case 'x': case 'X': 
            cancelSpellTargetSelection();
            std::cout << "Spell casting cancelled.\n";
            wanderer.addMana(spell->getManaCost());
            return;
    }
    
    if (moved) {
        int distance = std::max(std::abs(targetX - wandererPosition.x), 
                               std::abs(targetY - wandererPosition.y));
        
        if (distance <= spell->getRange()) {
            spell->cast(Position(targetX, targetY), field);
            selectingSpellTarget = false;
            selectedSpellIndex = -1;
            wandererCanMove = false;
        } else {
            std::cout << "Target out of range! Max range: " << spell->getRange() << "\n";
        }
    }
}

bool InputHandler::isSelectingSpellTarget() const {
    return selectingSpellTarget;
}

void InputHandler::startSpellTargetSelection(int spellIndex) {
    selectingSpellTarget = true;
    selectedSpellIndex = spellIndex;
    SpellCard* spell = wanderer.getHand().getSpell(spellIndex);
    std::cout << "Select target for " << spell->getName() 
              << " (WASD to move cursor, X to cancel)\n";
}

void InputHandler::cancelSpellTargetSelection() {
    selectingSpellTarget = false;
    selectedSpellIndex = -1;
}