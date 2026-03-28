#ifndef UNTITLED_PROGRESSION_H
#define UNTITLED_PROGRESSION_H

class GameState;
class Player;

class ProgressionSystem {
public:
    ProgressionSystem() = default;

    void applyLevelUp(GameState &state);

    void scaleEnemyStats(int level, int &outHp, int &outDmg) const;
};

#endif //UNTITLED_PROGRESSION_H

