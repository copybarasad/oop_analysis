#pragma once

#include "IGameObject.hpp"

// TODO: switch case not const error
// namespace screen_symbol {
//     extern const char PLAYER;
//     extern const char ENEMY;
//     extern const char ENEMY_BUILDING;
//     extern const char STONE;
//     extern const char SWAMP;
//     extern const char EMPTY;
// }

const int INF = 1e9;

class GameObject : public virtual IGameObject {
protected:
    char screen_symbol = '.'; //TODO(?) static

public:
    GameObject() = default;
    GameObject(char screen_symbol);

    char get_screen_symbol() const override;
    virtual GameObject* clone() const = 0;
};
