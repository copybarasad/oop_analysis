#ifndef SPELLAREASYSTEM_H
#define SPELLAREASYSTEM_H

#include <utility>

class Player;
class GameField;

class SpellAreaSystem {
public:
    static std::pair<int, int> findOptimalArea(const Player& caster, const GameField& field, int range, int areaSize = 2);
    static int countTargetsInArea(const Player& caster, int startX, int startY, int areaSize);
    static bool isPositionInArea(int posX, int posY, int areaX, int areaY, int areaSize);
};

#endif // SPELLAREASYSTEM_H