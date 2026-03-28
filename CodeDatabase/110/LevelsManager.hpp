#ifndef LEVELS_MANAGER_H
#define LEVELS_MANAGER_H

#include <map>

#include "../../GameBlock/GameLevel/GameLevel.hpp"
#include "../../Items/Item.hpp"

class LevelsManager {
    private:
    std::map<size_t, GameLevel*> levels;
    std::map<size_t, std::vector<Item*>> boosts;

    public:
    LevelsManager () = default;
    ~LevelsManager ();

    void addLevel (size_t index, GameLevel* level);
    void addBoost (size_t level, size_t index, Item* boost);
    GameLevel& getLevel (size_t index);
    std::map<size_t, GameLevel*> getLevels ();
    std::map<size_t, std::vector<Item*>> getBoosts ();
};

#endif