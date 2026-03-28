#ifndef AIM_HPP_INCLUDED
#define AIM_HPP_INCLUDED

#include "position.hpp"
#include "spell.hpp"

struct Aim {
    bool active{false};
    TargetMode mode{TargetMode::None};
    int radius{0};
    int areaSide{2};
    Position origin{0,0};
    Position cur{0,0};
    int cardIndex{-1};
};

#endif

