#include "GameRenderer.h"
#include "Player.h"
#include "GameField.h"
#include "DirectDamageSpell.h"  
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "MeleeSpell.h"
#include <iostream>
#include <iomanip>

// Отрисовка игрового поля
void GameRenderer::renderGameState(const GameField& field, const Player& player) const {
    std::cout << "\n=== GAME FIELD ===\n";

    // Отрисовка координат по X
    std::cout << "   ";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << std::setw(2) << x << " ";
    }
    std::cout << std::endl;

    // Отрисовка строк поля
    for (int y = 0; y < field.getHeight(); ++y) {
        std::cout << std::setw(2) << y << " ";
        for (int x = 0; x < field.getWidth(); ++x) {
            Position pos(x, y);
            Position player_pos = player.getPosition();

            // Определение символа для клетки
            if (player_pos.getX() == x && player_pos.getY() == y) {
                std::cout << " P ";
            }
            else if (field.hasTrapAt(x, y)) {
                std::cout << " T ";
            }
            else if (field.hasEnemyAt(x, y)) {
                std::cout << " E ";
            }
            else if (field.hasTowerAt(x, y)) {
                std::cout << " B ";
            }
            else if (field.hasObstacleAt(x, y)) {
                std::cout << " # ";
            }
            else {
                std::cout << " . ";
            }
        }
        std::cout << " " << std::setw(2) << y << std::endl;
    }

    // Отрисовка координат по X (низ)
    std::cout << "   ";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << std::setw(2) << x << " ";
    }
    std::cout << std::endl;
}

// Отрисовка статистики игрока
void GameRenderer::renderPlayerStats(const Player& player) const {
    std::cout << "\n=== PLAYER STATUS ===\n";
    std::cout << "Health: " << player.getHealth() << " | ";
    std::cout << "Mana: " << player.getMana() << "/" << player.getMaxMana() << " | ";
    std::cout << "Position: (" << player.getPosition().getX()
        << ", " << player.getPosition().getY() << ")\n";
}

// Отрисовка заклинаний в руке игрока (ЕДИНСТВЕННАЯ РЕАЛИЗАЦИЯ)
void GameRenderer::renderPlayerHand(const Player& player) const {
    std::cout << "\n=== SPELLS IN HAND (" << player.getHand().getSpellCount()
        << "/4 MAX) ===\n";

    const PlayerHand& hand = player.getHand();
    for (size_t i = 0; i < hand.getSpellCount(); ++i) {
        SpellCard* spell = hand.getSpell(i);
        if (spell) {
            std::cout << i + 1 << ". ";

            if (dynamic_cast<DirectDamageSpell*>(spell)) {
                std::cout << "Direct Damage";
                if (spell->isAreaSpell()) std::cout << " [AREA MODE]";
                std::cout << " (damage: " << spell->getDamage()
                    << ", mana: " << spell->getManaCost()
                    << ", range: " << spell->getCastRange() << ")\n";
            }
            else if (dynamic_cast<AreaDamageSpell*>(spell)) {
                std::cout << "Area Damage";
                if (spell->getCastRange() > 2) std::cout << " [EXTENDED RANGE]";
                std::cout << " (damage: " << spell->getDamage()
                    << ", mana: " << spell->getManaCost()
                    << ", range: " << spell->getCastRange() << ")\n";
            }
            else if (dynamic_cast<TrapSpell*>(spell)) {
                std::cout << "Trap";
                if (spell->isAreaSpell()) std::cout << " [MINE MODE]";
                std::cout << " (damage: " << spell->getDamage()
                    << ", mana: " << spell->getManaCost() << ")\n";
            }
            else if (dynamic_cast<MeleeSpell*>(spell)) {
                std::cout << "Melee";
                if (spell->getCastRange() > 0) std::cout << " [EXTENDED RANGE]";
                std::cout << " (damage: " << spell->getDamage()
                    << ", mana: " << spell->getManaCost() << ") - hits area around player\n";
            }
            else {
                std::cout << "Unknown Spell";
                std::cout << " (damage: " << spell->getDamage()
                    << ", mana: " << spell->getManaCost()
                    << ", range: " << spell->getCastRange() << ")\n";
            }
        }
    }
}