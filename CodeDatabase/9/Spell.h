#ifndef SPELL_H
#define SPELL_H

#include <utility>
#include <vector>

class Board;
class Player;
class Enemy;
class EnemyBase;
class EnemyTower;
class Ally;

struct SpellModifier{
    int radiusDelta;
    int aoeSizeDelta;
    int damageDelta;
    int summonCountDelta;

    SpellModifier(){
        radiusDelta = 0;
        aoeSizeDelta = 0;
        damageDelta = 0;
        summonCountDelta = 0;
    }
};

struct SpellContext{
    Board* board;
    Player* player;
    std::vector<Enemy>* enemies;
    EnemyBase* base;
    std::vector<EnemyTower>* towers;
    std::vector<Ally>* allies;
    std::pair<int,int> targetCell;
    bool hasTarget;

    SpellContext();
};

class ISpell{
public:
    virtual ~ISpell();

    virtual const char* GetName() const = 0;
    virtual bool NeedsTarget() const = 0;
    virtual int GetRange() const = 0;

    virtual bool CanCast(SpellContext& context) = 0;
    virtual bool Cast(SpellContext& context) = 0;

    virtual void ApplyModifier(const SpellModifier& mod) = 0;
    virtual bool ConsumesModifier() const = 0;

    virtual bool IsPersistent() const;
    virtual bool CanActivateFromKey() const;

    virtual ISpell* CloneNew() const = 0;
};

#endif