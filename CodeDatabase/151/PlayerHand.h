#pragma once
#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "Spell.h"
#include <vector>
#include <memory>

class PlayerHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;

public:
    PlayerHand(int maxSize);

    // запрет копирования (unique_ptr не копируется)
    PlayerHand(const PlayerHand&) = delete;
    PlayerHand& operator=(const PlayerHand&) = delete;

    // разрешение перемещения
    PlayerHand(PlayerHand&&) = default;
    PlayerHand& operator=(PlayerHand&&) = default;

    bool addSpell(std::unique_ptr<Spell> spell);
    bool removeSpell(int index);
    Spell* getSpell(int index) const;
    bool canCastSpell(int index, int playerMana) const;
    int getSize() const;
    int getMaxSize() const;
    bool isFull() const;

    // Получение случайного заклинания для начальной руки
    static std::unique_ptr<Spell> getRandomStarterSpell();
};

#endif