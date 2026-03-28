#ifndef PLAYER_H
#define PLAYER_H

#include "game_object.h"
#include "game_constants.h"
#include "spell_card.h"
#include "player_hand.h"
#include <vector>
#include <memory>
#include <iostream>

class Player : public GameObject {
public:
    Player();
    void move(int deltaX, int deltaY);

    int getScore() const;
    void addScore(int points);
    void setScore(int score);

    bool learnSpell(std::unique_ptr<SpellCard> spell);
    bool canLearnSpell() const;
    const PlayerHand& getHand() const;
    bool castSpell(int spellIndex);

    bool canAffordSpell(int cost) const;
    bool buySpell(std::unique_ptr<SpellCard> spell, int cost);
    bool removeSpell(int index);

    void save(std::ostream& out) const;
    void load(std::istream& in);

private:
    int score;
    PlayerHand hand;
};

#endif


