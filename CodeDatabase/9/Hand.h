#ifndef HAND_H
#define HAND_H

#include <vector>
#include "Spell.h"

class Hand{
private:
    std::vector<ISpell*> _cards;
    int capacity_;
    int selected_;

public:
    Hand(int capacity);
    ~Hand();
    Hand(const Hand& other);
    Hand& operator=(const Hand& other);

    int GetCapacity() const;
    int GetCount() const;
    int GetSelectedIndex() const;
    bool RemoveSelected();

    void SelectByIndex(int index);
    ISpell* GetSelected();

    bool AddCard(ISpell* card);
    void GetCardNames(std::vector<const char*>& out);
};

#endif