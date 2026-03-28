#ifndef UNTITLED_LEVELMANAGER_H
#define UNTITLED_LEVELMANAGER_H

#include "Coord.h"
#include "Field.h"

class GameState;

class LevelManager {
public:
    LevelManager() = default;

    void populateLevel(GameState &state);

    void generatePreset(GameState &state, int presetIndex);

private:
    void generateMapPreset(Field &field, int preset);
    int presetForLevel(int level) const;
};

#endif //UNTITLED_LEVELMANAGER_H
