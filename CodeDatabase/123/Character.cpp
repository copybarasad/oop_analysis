#include "Character.h"
#include <math.h>
#include <iostream>
#include <format>
using namespace std;

Character::Character(int maxLife, int melleDamage, int longRangeDamage) : Entity(maxLife), mMeleeCombatDamage(melleDamage), mLongRangeCombatDamage(longRangeDamage)
{
    mIsSlowed = false;
    mHitType = HitType::CloseCombat;
}

Character::Character(const Character &ch) : Entity(ch)
{
    mMeleeCombatDamage = ch.mMeleeCombatDamage;
    mLongRangeCombatDamage = ch.mLongRangeCombatDamage;
    mIsSlowed = ch.mIsSlowed;
    mHitType = ch.mHitType;
    mExp = ch.mExp;
}

Character::Character(Character &&ch) : Entity{ch}
{
    mMeleeCombatDamage = ch.mMeleeCombatDamage;
    mLongRangeCombatDamage = ch.mLongRangeCombatDamage;
    mIsSlowed = ch.mIsSlowed;
    mHitType = ch.mHitType;
    mExp = ch.mExp;
}

void Character::setMeleeDamage(int damage)
{
    mMeleeCombatDamage = damage;
}

int Character::getMeleeDamage() const
{
    return mMeleeCombatDamage;
}

void Character::setLongRangeCombatDamage(int dm)
{
    mLongRangeCombatDamage = dm;
}

int Character::getLongRangeCombatDamage() const
{
    return mLongRangeCombatDamage;
}

void Character::setSlowDown(bool s)
{
    string who;
    switch (mType)
    {
    case CharacterType::Player:
        who = "Player";
        break;
    case CharacterType::Enemy:
        who = "Enemy";
        break;
    case CharacterType::Building:
        who = "Building";
        break;
    }
    cout << std::format("{} slows {}", who, s) << endl;
    mIsSlowed = s;
}

bool Character::isSlowed() const
{
    return mIsSlowed;
}

void Character::setHitType(HitType type)
{
    mHitType = type;
}

HitType Character::getHitType() const
{
    return mHitType;
}

void Character::addExp(int exp)
{
    mExp += exp;
}

void Character::subExp(int exp)
{
    mExp -= exp;
}

int Character::getExp() const
{
    return mExp;
}

Character &Character::operator=(const Character &obj)
{
    Character tmp(obj);
    mMeleeCombatDamage = tmp.mMeleeCombatDamage;
    mLongRangeCombatDamage = tmp.mLongRangeCombatDamage;
    mIsSlowed = tmp.mIsSlowed;
    mHitType = tmp.mHitType;
    mExpForKill = tmp.mExpForKill;
    mExp = tmp.mExp;

    mPos = tmp.mPos;
    mLife = tmp.mLife;
    mMaxLife = tmp.mMaxLife;
    mType = tmp.mType;

    return *this;
}

Character &Character::operator=(Character &&obj)
{
    Character tmp{obj};
    mMeleeCombatDamage = tmp.mMeleeCombatDamage;
    mLongRangeCombatDamage = tmp.mLongRangeCombatDamage;
    mIsSlowed = tmp.mIsSlowed;
    mHitType = tmp.mHitType;
    mExpForKill = tmp.mExpForKill;
    mExp = tmp.mExp;

    mPos = tmp.mPos;
    mLife = tmp.mLife;
    mMaxLife = tmp.mMaxLife;
    mType = tmp.mType;

    return *this;
}
