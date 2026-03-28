#ifndef HAND_H
#define HAND_H

#include <vector>
#include <memory>
#include "ISpell.h"

class Hand {
public:
    Hand(size_t max_size);

    bool addSpell(std::unique_ptr<ISpell> spell);
    
    ISpell* getSpell(size_t index) const;
    
    size_t getSpellCount() const;
    
    void removeRandomSpells(int count);
    
    size_t getMaxSize() const;
    void setMaxSize(size_t size);
    void clear();

private:
    size_t max_size_;
    std::vector<std::unique_ptr<ISpell>> spells_;
};

#endif