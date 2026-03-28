#ifndef ENTITY
#define ENTITY

#include <vector>
#include "SFML/Graphics.hpp"
using namespace sf;
using Vec2 = sf::Vector2i;

enum class CharacterType
{
    Empty,
    Player,
    Enemy,
    Building
};

class Entity
{
protected:
    int mLife;
    int mMaxLife;
    Vec2 mPos;
    CharacterType mType = CharacterType::Empty;
    int mExpForKill = 0;

public:
    Entity(int maxLife);

    Entity(const Entity &e);

    Entity(Entity &&e);

    void setLife(int life);

    int getLife() const;

    void setMaxLife(int maxLife);

    void dealDamage(int dmg);

    bool isDead() const;

    int getMaxLife() const;

    void setPosition(Vec2 pos);

    Vec2 getPosition() const;

    CharacterType getType() const;

    int getExpForKill() const;

    Entity &operator=(const Entity &obj);

    Entity &operator=(Entity &&obj);

    ~Entity() = default;
};

#endif