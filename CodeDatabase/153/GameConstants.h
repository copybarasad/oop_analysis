#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

namespace GameConstants {
    const int PLAYER_START_HEALTH = 100;
    const int PLAYER_START_MANA = 50;
    const int PLAYER_MAX_MANA = 100;
    const int PLAYER_DAMAGE = 10;
    const int PLAYER_START_SCORE = 0;

    const int ENEMY_START_HEALTH = 30;
    const int ENEMY_DAMAGE = 5;

    const int SCORE_FOR_MOVE = 1;
    const int SCORE_FOR_KILL = 10;
    const int MANA_FOR_MOVE = 5;

    const int FIELD_WIDTH = 15;
    const int FIELD_HEIGHT = 15;

    const int STARTER_FIREBALL_COST = 8;
    const int STARTER_FIREBALL_RANGE = 2;
    const int STARTER_FIREBALL_DAMAGE = 15;

    const int STARTER_FIRESTORM_COST = 12;
    const int STARTER_FIRESTORM_RANGE = 1;
    const int STARTER_FIRESTORM_DAMAGE = 8;

    const int ICE_ARROW_COST = 40;
    const int ICE_ARROW_RANGE = 4;
    const int ICE_ARROW_DAMAGE = 20;

    const int ICE_STORM_COST = 90;
    const int ICE_STORM_RANGE = 3;
    const int ICE_STORM_DAMAGE = 12;
}

#endif