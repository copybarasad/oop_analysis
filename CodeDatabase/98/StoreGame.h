#ifndef SROTEGAME_H
#define SROTEGAME_H

#include "gamestate.h"
#include <string>

class StoreGame {
public:
    static bool SaveGame(const GameState& state, const std::string& filename);
    static bool LoadGame(GameState& state, const std::string& filename);
    static std::unique_ptr<ISpell> CreateSpellByName(const std::string& name);
};

#endif