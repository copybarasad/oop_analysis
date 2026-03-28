#ifndef UNTITLED_COMBATSYSTEM_H
#define UNTITLED_COMBATSYSTEM_H

#include <tuple>
#include <memory>

class Entity;
class GameModel;
struct Coord;

class CombatSystem {
public:
    std::pair<bool,bool> resolveAttack(std::shared_ptr<Entity> attacker,
                                       std::shared_ptr<Entity> defender,
                                       const Coord& defenderPos,
                                       GameModel& model);

    bool resolveTrap(std::shared_ptr<Entity> defender,
                     std::shared_ptr<Entity> trap,
                     const Coord& trapPos,
                     GameModel& model);
};

#endif // UNTITLED_COMBATSYSTEM_H
