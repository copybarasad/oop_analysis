#ifndef GAME_GAMELOGIC_H
#define GAME_GAMELOGIC_H

#include <nlohmann/json.hpp>
#include <utility>
#include <vector>
#include "Coordinates.h"
#include "Map.h"

class Object;
class  Player;

class GameLogic {
private:
    Player* player_ = nullptr;
    int score_ = 0;
    Map map_;
    std::vector<Object *> updateble_;

public:
    GameLogic(size_t width, size_t height, size_t layers,
              std::vector<Coordinates> blocks) : map_(width, height, layers, std::move(blocks)) {
    }

    GameLogic(size_t width, size_t height, size_t layers) : map_(width, height, layers) {
    }

    ~GameLogic();

    Map &getMap();

    void addScore(int points);

    int getScore() const;

    int addObject(Object *obj, Coordinates position);

    int removeObject(size_t id);

    void update() const;

    Player* getPlayer() const;

    void clear();

    void newMap(int width, int height);

    void saveJson(nlohmann::json *savefile) const;

    void setScore(int score);

};

#endif
