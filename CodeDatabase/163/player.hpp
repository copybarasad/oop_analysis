#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "game_object.hpp"
#include "i_attacker.hpp"
#include "player_hand.hpp"
#include <memory>

class Map;
class GameState;

enum class WeaponType {
    SWORD,
    BOW
};

class Player : public GameObject, public IAttacker {
private:
    int swordDamage;
    int bowDamage;
    WeaponType currentWeapon;
    bool isSlowed;
    bool hasMovedThisTurn;
    bool hasAttackedThisTurn;
    bool hasCastSpellThisTurn;
    std::unique_ptr<PlayerHand> spellHand;
    static constexpr int BOW_RANGE = 15;

    bool isValidDirection(int dirX, int dirY) const;

public:
    Player(int startX, int startY, Cell& cell, int hp, int range, 
           int swordDmg, int bowDmg, int handSize = 5);
    
    ~Player() override = default;
    
    void switchWeapon();
    WeaponType getCurrentWeapon() const;
    
    bool tryMove(int targetX, int targetY, Map& map);
    void startTurn();
    void endTurn();
    bool getIsSlowed() const;
    
    int getAttackDamage() const override;
    bool canAttack() const override;
    void performAttack(GameObject& target) override;
    void resetAttackFlag() override;
    
    PlayerHand* getSpellHand();
    const PlayerHand* getSpellHand() const;
    bool canCastSpell() const;
    void castSpell(int spellIndex, GameState* gameState, int targetX, int targetY);
    void resetSpellFlag();
    
    bool canAttackInDirection(int dirX, int dirY, const Map& map) const;
    int performDirectionalAttack(int dirX, int dirY, const Map& map,
                            const std::vector<std::unique_ptr<GameObject>>& enemies,
                            const std::vector<std::unique_ptr<GameObject>>& buildings = {});
    
    bool canMove() const;
    void resetTurnFlags() override;
    void takeTurn(Map& map) override;
    
    void enemyDefeated();
};

#endif