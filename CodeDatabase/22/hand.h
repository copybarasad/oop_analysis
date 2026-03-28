#pragma once
#include "spells/IspellCard.h"
#include <random>
#include <memory>
#include<iostream>

class Hand
{
private:

    std::vector<std::unique_ptr<ISpellCard>> spells_;
    int size_;

    public:
        Hand(int size);

        void addSpell(std::unique_ptr<ISpellCard> spell);
        void addSpell(std::string name);
        
        void deliteSpell(int index);
        void printHand();
        void generateRandomSpell();
        int size();

        std::unique_ptr<ISpellCard> chooseSpell();

        void cleanHand();

        const std::vector<std::unique_ptr<ISpellCard>>& getSpells() const;
        
        std::unique_ptr<ISpellCard> takeSpell(int index); // снимает и возвращает spell
        ISpellCard *peekSpell(int index);                 // возвращает raw pointer, не снимая
};
