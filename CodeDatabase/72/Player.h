//
// Created by bob on 10/4/25.
//

#ifndef GAME_TERM_PLAYER_H
#define GAME_TERM_PLAYER_H
#include <cstddef>

#include "Character.h"
#include "Hand.h"

#define DEFAULT_CLOSE_DAMAGE 30
#define DEFAULT_RANGED_DAMAGE 15

enum class FightMode { CloseCombat, RangedCombat};

class Player: public Character, public ISerializable{

    int points;
    FightMode mode;
    bool skipNextStep;
    int closeCombatDamage;
    int rangedCombatDamage;
    std::shared_ptr<Hand> hand;
    int healthMax;

public:

    explicit Player(std::shared_ptr<Hand> hand = nullptr, const int & closeCombatDamage = DEFAULT_CLOSE_DAMAGE, const int & rangedCombatDamage = DEFAULT_RANGED_DAMAGE,
        const Position & position = DEFAULT_POSITION, const int & health = DEFAULT_HEALTH);

    explicit Player(const std::size_t & points, const FightMode & mode, const bool & skipNextStep,
        const int & closeCombatDamage, const int & rangeCombatDamage,  std::shared_ptr<Hand> hand, const int&  healthMax, const Position & position);

    void setCloseCombatDamage(const int & closeCombatDamage);

    void setRangedCombatDamage(const int & rangedCombatDamage);

    void switchFightMode();

    int getScore() const;

    FightMode getFightMode() const;

    bool isSkipStep() const;

    void setSkipStep(const bool & isSkip);

    void addPoints(const int & points);

    std::shared_ptr<Hand> getHand();

    void restoreHealth();

    int getCloseDamage() const;

    int getRangeDamage() const;

    int getMaxHealth() const;

    void save(Saver &saver) const override;

    void load(Reader &reader) override;
};


#endif //GAME_TERM_PLAYER_H