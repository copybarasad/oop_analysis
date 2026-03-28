#ifndef CONFIG_H
#define CONFIG_H

enum PlayerConfig{
    PLAYER_HP = 10,
    PLAYER_MELEE_DAMAGE = 2,
    PLAYER_RANGED_DAMAGE = 1
};

enum EnemyConfig{
    ENEMY_HP = 3,
    ENEMY_DAMAGE = 1
};

enum GameRules{
    RANGED_RANGE = 3,
    BASE_SPAWN_DELAY = 8
};

enum SpellConfig{
    HAND_CAPACITY = 5,

    DIRECT_RANGE = 4,
    DIRECT_DAMAGE = 2,

    AOE_RANGE = 4,
    AOE_DAMAGE = 1,
    AOE_SIZE   = 2,
    SPELL_BUY_COST =30
};


enum SpellTrapConfig{
    TRAP_RANGE = 4,
    TRAP_DAMAGE = 2
};

enum TowerConfig{
    TOWER_RANGE = 3,
    TOWER_DAMAGE = 1,
    TOWER_COOLDOWN = 1,
    TOWER_HP = 1
};

enum AllyConfig{
    ALLY_HP = 4,
    ALLY_DAMAGE = 1
};

enum EmpowerConfig{
    EMPOWER_RADIUS_DELTA = 1,
    EMPOWER_AOE_DELTA = 1,
    EMPOWER_DAMAGE_DELTA = 1,
    EMPOWER_SUMMON_DELTA = 1
};




#endif