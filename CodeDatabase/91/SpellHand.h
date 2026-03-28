#ifndef SPELL_HEND_H
#define SPELL_HEND_H

#include "ISpell.h"
#include <vector>
#include <memory>

class Game;

class SpellHand {
    private:
        std::vector<std::unique_ptr<ISpell>> spells;
        size_t maxSize;

    public:
        SpellHand(size_t maxSpells = 3);

        bool addSpell(std::unique_ptr<ISpell> spell);
        bool removeSpell(int index);
        bool castSpell(int index, Game& game, const Position& target);
        void removeRandomSpells(size_t count);

        //   === Getters ===
        const ISpell* getSpell(int index) const;
        int getSpellCount() const;
        size_t getMaxSize() const;
        bool isFull() const;
        bool isEmpty() const;

        void upgradeMaxSize(int amount);
        void upgradeSpellAt(int index);

        void displayHand() const;

        // === JSON ===
        const std::vector<std::unique_ptr<ISpell>>& getSpells() const;

        void loadState(const nlohmann::json& j);
};

#endif