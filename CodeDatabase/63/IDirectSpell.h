#ifndef RPG_IDIRECTSPELL_H
#define RPG_IDIRECTSPELL_H

#include <utility>
class Entity;
class GameField;
class EntityManager;

class IDirectSpell {
public:
    virtual ~IDirectSpell() = default;

    virtual bool canHitTarget(std::pair<int,int>& coordinates, Entity* caster, GameField& field, EntityManager& entityManager) = 0;
};

#endif