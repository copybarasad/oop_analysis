#pragma once
#include "Colors.h"

namespace Config {
    namespace Player {
         constexpr char SPRITE = 'P';
         const std::string COLOR = TermColors::GREEN;
         const int HEALTH = 100;
         const int DAMAGE = 10;
         const int SPEED = 1;
         const int HAND_SIZE = 5;
         const int RANGE_DAMAGE = 100;
         const int RANGE = 4;
    }

    namespace Wall {
        constexpr char SPRITE = '#';
        const std::string COLOR = TermColors::MAGENTA;
    }

    namespace Enemy {
        constexpr char SPRITE = 'E';
        const std::string COLOR = TermColors::GRAY_MAGENTA;
        const int HEALTH = 30;
        const int DAMAGE = 5;
        const int SPEED = 1;
        const int AGGRO_RANGE = 10;
    }
}
