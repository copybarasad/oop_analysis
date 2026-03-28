#pragma once
#include "../game_spells/Spell.h"
#include <vector>
#include <memory>
#include <random>

class PlayerHand {
private:
    std::vector<std::shared_ptr<Spell>> spells;
    size_t maxSize;
    bool isValidIndex(size_t index) const;

public:
    PlayerHand(size_t maxHandSize);
    ~PlayerHand() = default;

    // Методы для управления заклинаниями
    bool addSpell(std::shared_ptr<Spell> spell);
    bool removeSpell(size_t index);
    bool useSpell(size_t index, Player& player, GameState& state, int targeX, int targetY);
    void addRandomSpell(std::shared_ptr<Spell> spell);
    void clearHand();

    // Геттеры
    std::shared_ptr<const Spell> getSpell(size_t index) const;
    const std::vector<std::shared_ptr<Spell>>& getAllSpells() const;  
    bool isFull() const;
    bool isEmpty() const;
    size_t getCurrentSize() const;
    size_t getMaxSize() const;

    // Метод для улучшения
    bool upgradeSpell(size_t index);
};