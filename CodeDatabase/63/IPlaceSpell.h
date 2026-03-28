#ifndef RPG_IPLACESPELL_H
#define RPG_IPLACESPELL_H

#include <utility>
class GameField;
class EntityManager;

class IPlaceSpell {
public:
    virtual ~IPlaceSpell() = default;

    [[nodiscard]] virtual bool canBePlaced(std::pair<int,int>& coordinates, GameField& field, EntityManager& entityManager) const = 0;
};

#endif
