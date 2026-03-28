#pragma once
#include <sstream>
#include "spell.h"
using namespace std;
class ConsoleRenderer;

class Hand {
private:
    vector<Spell*> cards;
    int maxSize;
    
public:
    Hand(int size) : maxSize(size) {}
    ~Hand();
    
    void clearHand();
    bool addSpell(Spell* spell, string& message);
    bool addSpell(Spell* spell);
    bool removeSpell(int index);
    bool useSpell(int idx, int x, int y, Field& field, Player& player, Enemy& enemy, string& message);
    bool buySpell(int spellIndex, int& playerPoints, string& message);
    void showHand(string& outputMessage) const;
    void removeHalfSpells();
    
    int getSpellCount() const;
    bool isFull() const;
    bool isEmpty() const;
    int getMaxSize() const;
    Spell* getSpell(int index) const;
    const vector<Spell*>& getAllSpells() const;
};