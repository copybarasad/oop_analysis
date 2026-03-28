//
// Created by Artem on 22.09.2025.
//

#ifndef LABA1_PLAYER_H
#define LABA1_PLAYER_H

#include <utility>

#include "Creature.h"
#include "Attacks/LongRangeAttack.h"
#include "Attacks/CloseRangeAttack.h"
#include "Attacks/Spels/Hand/SpellHand.h"

class Player : public Creature {
public:
    Player(int xCoordinate, int yCoordinate, int healthPoint, bool isDisabledFlag, int stepRange,
           CloseRangeAttack closeRangeAttack, LongRangeAttack longRangeAttack,
           bool isCloseRangeAttackSelectedFlag, long score, SpellHand hand) : Creature(xCoordinate, yCoordinate, healthPoint, EntityType::PlayerEnt, isDisabledFlag,stepRange),
                                                                                     closeRangeAttack(std::move(closeRangeAttack)),
                                                                                     longRangeAttack(std::move(longRangeAttack)),
                                                                                     isCloseRangeAttackSelectedFlag(isCloseRangeAttackSelectedFlag),
                                                                                     score(score), hand(std::move(hand)) {}

    Player(Player &&other) noexcept
            : Creature(std::move(other)),
              closeRangeAttack(std::move(other.closeRangeAttack)),
              longRangeAttack(std::move(other.longRangeAttack)),
              isCloseRangeAttackSelectedFlag(other.isCloseRangeAttackSelectedFlag),
              score(other.score),
              hand(std::move(other.hand)) {}

    Player(const Player &other)
            : Creature(other),
              closeRangeAttack(other.closeRangeAttack),
              longRangeAttack(other.longRangeAttack),
              isCloseRangeAttackSelectedFlag(other.isCloseRangeAttackSelectedFlag),
              score(other.score),
              hand(other.hand) {}

    virtual const CloseRangeAttack &getCloseRangeAttack() const noexcept final;
    virtual const LongRangeAttack &getLongRangeAttack() const noexcept final;

    void setCloseRangeAttack(const CloseRangeAttack &closeRangeAttack);
    void setLongRangeAttack(const LongRangeAttack &longRangeAttack);

    virtual bool isCloseRangeAttackSelected() const noexcept final;
    virtual void changeSelectedAttackType() noexcept final;

    virtual long getScore() const noexcept final;
    virtual void setScore(int score) noexcept final;
    virtual SpellHand& getSpellHand() noexcept final;


    std::unique_ptr<Entity> clone() const override;
    std::string serialize() override;
    static Player* deserialize(std::map<std::string, std::string> json, SpellFactory& spellFactory) noexcept;

protected:
    CloseRangeAttack closeRangeAttack;
    LongRangeAttack longRangeAttack;
    bool isCloseRangeAttackSelectedFlag;
    long score;
    SpellHand hand;
};


#endif //LABA1_PLAYER_H
