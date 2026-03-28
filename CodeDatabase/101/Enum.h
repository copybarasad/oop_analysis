#ifndef ENUM_H
#define ENUM_H


enum class EventType {

    GAME_STARTED,
    GAME_ENDED,
    LEVEL_STARTED,
    LEVEL_COMPLETED,


    PLAYER_MOVED,
    PLAYER_ATTACKED,
    PLAYER_DAMAGED,
    PLAYER_HEALED,
    PLAYER_DIED,
    PLAYER_MONEY_CHANGED,
    PLAYER_SPELL_RECEIVED,


    ENEMY_SPAWNED,
    ENEMY_ATTACKED,
    ENEMY_DAMAGED,
    ENEMY_DIED,


    TOWER_DAMAGED,
    TOWER_DESTROYED,


    SPELL_CAST,
    SPELL_PURCHASED,


    UPGRADE_PURCHASED,


    SCORE_CHANGED,
    ERROR_OCCURRED
};


enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERR
};

#endif // ENUM_H
