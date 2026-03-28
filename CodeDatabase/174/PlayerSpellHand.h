#ifndef PLAYER_SPELL_HAND_H
#define PLAYER_SPELL_HAND_H

#include "Spell.h"
#include "Constants.h"
#include <vector>
#include <memory>


class Game;
class GameField;
class Position;
class Player;

class PlayerSpellHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    size_t maxHandSize;
    int buffCount = 0;

public:
    PlayerSpellHand(size_t maxHandSize = DEFAULT_HAND_SIZE);
    bool addSpell(std::unique_ptr<Spell> spell);
    bool useSpell(size_t index, GameField& field, const Position& target, Player& player, Game& game);
    const std::vector<std::unique_ptr<Spell>>& getSpells() const;
    static std::unique_ptr<Spell> generateRandomSpell();
    static std::unique_ptr<Spell> createSpellByName(const std::string& name);

    void incrementBuffCount();
    int getBuffCount() const;
    void removeHalfOfSpells();
    void resetBuffCount();
    void clear();
    
    void increaseMaxHandSize(size_t amount);
    size_t getMaxHandSize() const;
};

#endif