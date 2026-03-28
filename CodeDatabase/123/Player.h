#ifndef PLAYER
#define PLAYER

#include "Hand.h"
#include "Character.h"
#include "Spells/Spell.h"

class Player : public Character
{
private:
    Hand *mHand;

public:
    Player(const Player &p);

    Player(Player &&p);

    Player(int maxLife = 50, int meleeDamage = 10, int longRangeDamage = 5);

    Hand *getHand() const;

    void addSpell();

    Spell *getSpell(int i);

    int getSize() const;

    void setSelection(int sel);

    int getSelection() const;

    Player &operator=(const Player &obj);

    Player &operator=(Player &&obj);

    ~Player();
};

#endif