#pragma once
#include <vector>
#include <memory>


class FieldCell;
class EntityManager;

struct GameContext {
    std::vector<FieldCell>& cells;
    EntityManager& entityManager;

    GameContext(std::vector<FieldCell>& c, EntityManager& em)
        : cells(c), entityManager(em) {}
};