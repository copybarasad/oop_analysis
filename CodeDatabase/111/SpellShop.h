#pragma once
#include "SpellHand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include <vector>
#include <memory>

class SpellShop {
private:
    SpellHand* playerSpellHand; // Используем указатель вместо ссылки
    std::vector<std::unique_ptr<Spell>> availableSpells;

public:
    SpellShop(SpellHand& spellHand);
    ~SpellShop() = default;
    
    // Удаляем копирование
    SpellShop(const SpellShop&) = delete;
    SpellShop& operator=(const SpellShop&) = delete;
    
    // Разрешаем перемещение
    SpellShop(SpellShop&& other) noexcept
        : playerSpellHand(other.playerSpellHand),
          availableSpells(std::move(other.availableSpells)) {
        other.playerSpellHand = nullptr;
    }
    
    SpellShop& operator=(SpellShop&& other) noexcept {
        if (this != &other) {
            playerSpellHand = other.playerSpellHand;
            availableSpells = std::move(other.availableSpells);
            other.playerSpellHand = nullptr;
        }
        return *this;
    }
    
    void openShop();
    void giveRandomStarterSpell();
    
    // Метод для обновления указателя на SpellHand
    void updateSpellHand(SpellHand& spellHand) {
        playerSpellHand = &spellHand;
    }

private:
    void initializeSpells();
    void displayShop() const;
    bool purchaseSpell(int spellIndex);
};