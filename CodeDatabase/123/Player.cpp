#include "Player.h"

Player::Player(const Player &p) : Character(p)
{
    mHand = new Hand(*p.mHand);
}

Player::Player(Player &&p) : Character{p}
{
    mHand = new Hand(*p.mHand);
}

Player::Player(int maxLife, int meleeDamage, int longRangeDamage) : Character(maxLife, meleeDamage, longRangeDamage)
{
    mType = CharacterType::Player;
    mHitType = HitType::CloseCombat;
    mExp = 0;
    mHand = new Hand(SPELL_CAPASITY);
}

Hand *Player::getHand() const
{
    return mHand;
}

Player &Player::operator=(const Player &obj)
{
    Player tmp(obj);
    mExp = tmp.mExp;

    mMeleeCombatDamage = tmp.mMeleeCombatDamage;
    mLongRangeCombatDamage = tmp.mLongRangeCombatDamage;
    mIsSlowed = tmp.mIsSlowed;
    mHitType = tmp.mHitType;
    mExpForKill = tmp.mExpForKill;

    mPos = tmp.mPos;
    mLife = tmp.mLife;
    mMaxLife = tmp.mMaxLife;
    mType = tmp.mType;

    mHand = new Hand(*obj.mHand);

    return *this;
}

Player &Player::operator=(Player &&obj)
{
    Player tmp{obj};
    mExp = tmp.mExp;

    mMeleeCombatDamage = tmp.mMeleeCombatDamage;
    mLongRangeCombatDamage = tmp.mLongRangeCombatDamage;
    mIsSlowed = tmp.mIsSlowed;
    mHitType = tmp.mHitType;
    mExpForKill = tmp.mExpForKill;

    mPos = tmp.mPos;
    mLife = tmp.mLife;
    mMaxLife = tmp.mMaxLife;
    mType = tmp.mType;

    mHand = new Hand(*obj.mHand);

    return *this;
}

void Player::addSpell()
{
    mHand->addSpell();
}

Spell *Player::getSpell(int i)
{
    return mHand->getSpell(i);
}

int Player::getSize() const
{
    return mHand->getSize();
}

void Player::setSelection(int sel)
{
    mHand->setSelection(sel);
}

int Player::getSelection() const
{
    return mHand->getSelection();
}

Player::~Player()
{
    delete mHand;
}
