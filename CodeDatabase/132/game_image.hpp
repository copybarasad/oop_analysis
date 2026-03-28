#pragma once

#include "field_image.hpp"

class GameImage {
public:
    int score;
    int coins;
    int time;
    int lvl;
    int cap_lvl;
    int hp_lvl;

    FieldImage field_img;

    GameImage(int score, int coins, int time, int lvl, int cap_lvl, int hp_lvl, FieldImage field_img):
        score(score),
        coins(coins),
        time(time),
        lvl(lvl),
        cap_lvl(cap_lvl),
        hp_lvl(hp_lvl),
        field_img(field_img)
    {}
};