#ifndef ENUMS_H
#define ENUMS_H

enum class Cell_type{
    EMPTY,
    BLOCKED,
    SLOW,
    BUILDING
};

enum class Move_status{
    SUCCESS,
    BLOCKED,
    SLOWED,
    ATTACKED
};

enum class Combat_mode{
    MELEE,
    RANGED
};

enum class Spell_type{
    DIRECT_DAMAGE,
    AREA_DAMAGE,
    TRAP,
    SUMMON,
    BUFF
};

enum class Target_type{
    ENEMY,
    BUILDING,
    AREA,
    SELF,
    NONE
};

enum class Event_type {
    PLAYER_MOVED,
    PLAYER_ATTACKED,
    PLAYER_CAST_SPELL,
    PLAYER_HEALED,
    PLAYER_DAMAGED,
    ENEMY_KILLED,
    ENEMY_SPAWNED,
    BUILDING_DESTROYED,
    SPELL_LEARNED,
    GAME_SAVED,
    GAME_LOADED,
    LEVEL_CHANGED,
    TRAP_TRIGGERED
};

enum class Log_level {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

enum class Log_output {
    FILE_ONLY,
    CONSOLE_ONLY,
    BOTH
};

#endif