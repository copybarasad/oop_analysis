#ifndef SPELL_MANAGER_H
#define SPELL_MANAGER_H

#include "player_hand.h"
#include "spell.h"
#include <vector>
#include <memory>

class GameField;
class Player;
class Enemy;
class EnemyBuilding;
class EnemyTower;


class SpellManager {
private:
    PlayerHand playerHand;
    int enhancementLevel;
    
public:
    explicit SpellManager(size_t maxHandSize = 3);
    bool addSpellToHand(std::unique_ptr<Spell> spell);
    PlayerHand& getHand();
    const PlayerHand& getHand() const;

    bool useSpell(int spellIndex, Player& player, GameField& field,
                  std::vector<Enemy>& enemies, 
                  std::vector<EnemyBuilding>& buildings,
                  std::vector<EnemyTower>& towers,
                  int targetX, int targetY);
    
    bool canUseSpell(int spellIndex, const Player& player, 
                     const GameField& field,
                     const std::vector<Enemy>& enemies,
                     const std::vector<EnemyBuilding>& buildings,
                     const std::vector<EnemyTower>& towers,
                     int targetX, int targetY) const;
    
    Spell* getSpellAt(int index) const;
    int getEnhancementLevel() const;
    void setEnhancementLevel(int lvl);
    void resetEnhancements();
};

#endif