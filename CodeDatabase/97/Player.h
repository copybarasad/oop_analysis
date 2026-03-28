#ifndef UNTITLED_PLAYER_H
#define UNTITLED_PLAYER_H

#include "Entity.h"
#include "Hand.h"
#include <memory>

class Player : public Entity, public std::enable_shared_from_this<Player> {
public:
    Player(int maxHealth = 100, int nearDamage = 30, int farDamage = 3);
    ~Player() override = default;

    std::shared_ptr<Entity> clone() const override;

    Hand& hand();
    const Hand& hand() const;

    int hpMax() const noexcept;

    void switchMode();
    int getDamageForCurrentMode() const noexcept;

    void takeDamage(int d) override;

    void addScore(int s);
    int score() const noexcept;

    void accumulateUpgrade(int r, int w, int h, int trapD, int spawnN);

    void clearAccumulation();

    bool isNear() const noexcept;
    std::string modeName() const;

    void setSkipNextMove(bool v);
    bool consumeSkipMove();
    bool hasSkipNextMove() const noexcept;

    void increaseHpMax(int add);
    void increaseNearDamage(int add);
    void increaseFarDamage(int add);
    void setHandCapacity(size_t newCap);
    void increaseHandCapacity(size_t add);
    void restoreToFullHp();
    void setHp(int hp);

    int getNearDamage() const { return nearDamage_; }
    int getFarDamage() const { return farDamage_; }

private:
    Hand hand_;
    int hpMax_;
    bool isNear_;
    int nearDamage_;
    int farDamage_;
    int score_;
    bool skipNextMove_;
};

#endif //UNTITLED_PLAYER_H

