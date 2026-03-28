#ifndef SPELL_CARD_HPP
#define SPELL_CARD_HPP

#include <string>
#include <memory>

class GameState;
class Map;

class SpellCard {
protected:
    std::string name;
    std::string description;
    int range;

public:
    SpellCard(const std::string& spellName, const std::string& spellDesc, 
              int spellRange);
    virtual ~SpellCard() = default;
    
    std::string getName() const;
    std::string getDescription() const;
    int getRange() const;
    
    virtual void castAsProjectile(GameState* gameState, int dirX, int dirY) const = 0;
    virtual std::unique_ptr<SpellCard> clone() const = 0;
};

#endif