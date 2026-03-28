#ifndef ENTITY_M_H
#define ENTITY_M_H

#include <vector>
#include <map>

#include "../../Field/Cell/Cell.hpp"

class EntityManager {
    private:
    unsigned int entityCounter;
    std::map<size_t, Cell*> entityTracker;     // 0 - player, 100 - ally, 200 - trap, 300 - enemy, 400 - building, 500 - tower
    std::map<size_t, size_t> typeCounter;

    public:
    EntityManager ();
    ~EntityManager () = default;

    void addTrack (Cell* trackingCell);
    void removeTrack (size_t id);

    size_t getEntityNumber () const;
    size_t getEnemiesNumber () const;
    const std::map<size_t, size_t>& getTypeCounter() const;
    std::map<size_t, Cell*>& getTracker ();
    Cell& getPlayerPos ();

    void updateTrack (size_t id, Cell& trackingCell);
    void deleteEntities ();
};

#endif