#include "Entity.h"
#include <iostream>
#include <format>
using namespace std;

Entity::Entity(int maxLife) : mPos(0, 0), mMaxLife(maxLife)
{
    mLife = maxLife;
    // cout << "Constructor " << classNum++ << endl;
}

Entity::Entity(const Entity &e)
{
    mLife = e.mLife;
    mMaxLife = e.mMaxLife;
    mPos = e.mPos;
    mType = e.mType;
    mExpForKill = e.mExpForKill;
    // cout << "Constructor " << classNum++ << endl;
}

Entity::Entity(Entity &&e)
{
    mLife = e.mLife;
    mMaxLife = e.mMaxLife;
    mPos = e.mPos;
    mType = e.mType;
    mExpForKill = e.mExpForKill;
    // cout << "Constructor " << classNum++ << endl;
}

void Entity::setLife(int life)
{
    mLife = life;
}

int Entity::getLife() const
{
    return mLife;
}

void Entity::setMaxLife(int maxLife)
{
    mMaxLife = maxLife;
}

int Entity::getMaxLife() const
{
    return mMaxLife;
}

void Entity::dealDamage(int dmg)
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
    cout << std::format("{} gets damage {}", who, dmg) << endl;
    mLife = std::max(0, mLife - dmg);
}

bool Entity::isDead() const
{
    return mLife == 0;
}

void Entity::setPosition(Vec2 pos)
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
    cout << std::format("{} moves from ({}, {}) to ({}, {})", who, mPos.x, mPos.y, pos.x, pos.y) << endl;
    mPos = pos;
}

Vec2 Entity::getPosition() const
{
    return mPos;
}

CharacterType Entity::getType() const
{
    return mType;
}

int Entity::getExpForKill() const
{
    return mExpForKill;
}

Entity &Entity::operator=(const Entity &obj)
{
    Entity tmp(obj);

    mPos = tmp.mPos;
    mLife = tmp.mLife;
    mMaxLife = tmp.mMaxLife;
    mType = tmp.mType;
    mExpForKill = tmp.mExpForKill;
    return *this;
}

Entity &Entity::operator=(Entity &&obj)
{
    Entity tmp(obj);

    mPos = tmp.mPos;
    mLife = tmp.mLife;
    mMaxLife = tmp.mMaxLife;
    mType = tmp.mType;
    mExpForKill = tmp.mExpForKill;
    return *this;
}