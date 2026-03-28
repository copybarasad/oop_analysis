#include "Enemy.h"

Enemy::Enemy(const Enemy &e) : Character(e)
{
}

Enemy::Enemy(int maxLife, int meleeDamage, int longRangeDamage) : Character(maxLife, meleeDamage, longRangeDamage)
{
    mType = CharacterType::Enemy;
    mExpForKill = 10;
}

// Enemy &Enemy::operator=(const Enemy &obj)
// {
//     Enemy tmp(obj);

//     mMeleeCombatDamage = tmp.mMeleeCombatDamage;
//     mLongRangeCombatDamage = tmp.mLongRangeCombatDamage;
//     mIsSlowed = tmp.mIsSlowed;
//     mHitType = tmp.mHitType;
//     mExpForKill = tmp.mExpForKill;

//     mPos = tmp.mPos;
//     mLife = tmp.mLife;
//     mMaxLife = tmp.mMaxLife;
//     mType = tmp.mType;

//     return *this;
// }
