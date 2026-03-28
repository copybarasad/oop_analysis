#ifndef PLAYERHAND_H
#define PLAYERHAND_H

#include <vector>
#include <memory>
#include <fstream>
#include "ISpell.h"

class PlayerHand {
public:
    PlayerHand(size_t maxSize);
    bool addSpell(std::unique_ptr<ISpell> spell);
    ISpell* getSpell(size_t index);
    void displaySpells() const;
    size_t getSpellCount() const;
    void removeSpell(size_t index);
    void removeRandomSpells(int count);
    void save(std::ofstream& out) const;
    void load(std::ifstream& in);

private:
    std::vector<std::unique_ptr<ISpell>> spells;
    const size_t maxSize;
};

#endif