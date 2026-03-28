#ifndef CHARACTER
#define CHARACTER

#include "Entity.h"

enum class HitType
{
    CloseCombat,
    LongRangeCombat,
    Hand
};

class Character : public Entity
{
protected:
    int mMeleeCombatDamage;
    int mLongRangeCombatDamage;
    bool mIsSlowed;
    HitType mHitType;
    int mExp = 0;

public:
    Character(int maxLife = 50, int melleDamage = 10, int longRangeDamage = 5);

    Character(const Character &ch);

    Character(Character &&ch);

    void setMeleeDamage(int damage);

    int getMeleeDamage() const;

    void setLongRangeCombatDamage(int dm);

    int getLongRangeCombatDamage() const;

    void setSlowDown(bool s);

    bool isSlowed() const;

    void setHitType(HitType type);

    HitType getHitType() const;

    void addExp(int exp);

    void subExp(int exp);

    int getExp() const;

    Character &operator=(const Character &obj);

    Character &operator=(Character &&obj);
};

#endif