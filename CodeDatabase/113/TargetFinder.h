#ifndef TARGETFINDER_H
#define TARGETFINDER_H

#include <vector>
#include <utility>

class Player;
class GameField;
class GameObject;

class TargetFinder {
public:
    static std::vector<GameObject*> findTargetsInRange(const Player& caster, const GameField& field, int range);
    static GameObject* findClosestTarget(const Player& caster, const GameField& field, int range);
    static std::pair<int, int> findBestAreaForDamage(const Player& caster, const GameField& field, int range);
    static std::vector<std::pair<int, int>> findAvailableTrapPositions(const Player& caster, const GameField& field, int range);
};

#endif // TARGETFINDER_H