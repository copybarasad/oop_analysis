#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace GameConstants {
    // Field
    constexpr int MIN_FIELD_SIZE = 10;
    constexpr int MAX_FIELD_SIZE = 25;
    constexpr int DEFAULT_PIT_COUNT = 15;
    constexpr int SPAWN_ATTEMPTS_LIMIT = 100;
    constexpr int MAX_CNT_ENEMY = 15;
    constexpr int MAX_CNT_PIT = 99;
    constexpr int MAX_CNT_BARRACKS = 3;
    constexpr int MAX_CNT_SALOON = 5;
    constexpr int MAX_CNT_COWBOYTOWER = 4;
    constexpr int MAX_CNT_TRAPS = 5;
    
    // Player
    constexpr int BEGIN_LEVEL_PLAYER = 1;
    constexpr int DEFAULT_PLAYER_HEALTH = 120;
    constexpr int DEFAULT_PLAYER_DAMAGE = 20;
    constexpr int DEFAULT_MELEE_DAMAGE = 30;
    constexpr int DEFAULT_RANGED_DAMAGE = 7;
    constexpr int DEFAULT_ATTACK_RANGE = 3;
    constexpr int XP_PER_LEVEL = 100;
    constexpr int HEALTH_PER_LEVEL = 20;
    constexpr int COINS_PER_KILL = 10;
    constexpr int SCORE_PER_KILL = 1;
    constexpr int XP_PER_KILL = 50;
    
    // Enemy
    constexpr int DEFAULT_ENEMY_HEALTH = 10;
    constexpr int DEFAULT_ENEMY_DAMAGE = 20;
    
    // Barracks
    constexpr int BARRACKS_HEALTH = 100;
    constexpr int BARRACKS_ENEMY_HEALTH = 20;
    constexpr int BARRACKS_ENEMY_DAMAGE = 25;
    constexpr int DEFAULT_COOLDOWN = 3;
    constexpr int RETRY_COOLDOWN = 1;
    constexpr int XP_PER_BARRACKS_DESTROY = 50;
    constexpr int COINS_PER_BARRACKS_DESTROY = 20;
    constexpr int SCORE_PER_BARRACKS_DESTROY = 3;

    // CowboyTower
    constexpr int TOWER_HEALTH = 80;
    constexpr int TOWER_ATTACK_DAMAGE = 15;
    constexpr int TOWER_ATTACK_COOLDOWN = 2;
    constexpr int TOWER_ATTACK_RANGE = 2;
    constexpr int XP_PER_TOWER_DESTROY = 50;
    constexpr int COINS_PER_TOWER_DESTROY = 20;
    constexpr int SCORE_PER_TOWER_DESTROY = 3;

    
    // Saloon
    constexpr int DEFAULT_FREEZE_TIME = 1;
    constexpr int HEAL_IN_SALOON = 20;

    // Spell
    constexpr int MAX_SPELL_LEVEL = 3;
    constexpr int BEGIN_SPELL_LEVEL = 1;

    // HandSpell
    constexpr int MAX_SIZE_HANDSPELL = 30;

    // BulletRain

    constexpr int BEGIN_LEVEL_BULLETRAIN = 1;
    constexpr int BULLETRAIN_DAMAGE = 100;
    constexpr int BEGIN_BULLETRAIN_AREA_OF_EFFECT = 1;
    constexpr int AREA_OF_EFFECT_PER_LEVEL = 1;
    constexpr int COST_BULLETRAIN = 3;

    // MegaShotgun
    
    constexpr int BEGIN_LEVEL_MEGASHOTGUN = 1;
    constexpr int MEGASHOTGUN_DAMAGE = 30;
    constexpr int BEGIN_MEGASHOTGUN_SIZE_ZONE_ATTACK = 1;
    constexpr int SIZE_ZONE_ATTACK_PER_LEVEL = 1;
    constexpr int COST_MEGASHOTGUN = 5;

    // CheapWhiskey:
    constexpr int CHEAPWHISKEY_DAMAGE = 30;
    constexpr int BEGIN_LEVEL_CHEAPWHISKEY = 1;
    constexpr int BEGIN_CHEAPWHISKEY_PLACEMENT_RANGE = 1;
    constexpr int DEFAULT_CHEAPWHISKEY_PLACEMENT_RANGE = 1;
    constexpr int CHEAPWHISKEY_PLACEMENT_RANGE_PER_LEVEL = 1;
    constexpr int COST_CHEAPWHISKEY = 7;

    // JohnnysGhost
    constexpr int HEALTH_JOHNNY = 50;
    constexpr int DAMAGE_GHOSTJOHNNY = 20;
    constexpr int BEGIN_MAX_CNT_GHOSTJOHNNY = 1;
    constexpr int MAX_CNT_GHOSTJOHNNY_PER_LEVEL = 1;
    constexpr int BEGIN_LEVEL_GHOSTJOHNNY = 1;
    constexpr int COST_JOHNYSGHOST = 30;
    constexpr int DEFAULT_RANGE_SPAWN = 1;

    // Update Spell
    constexpr int COST_UPDATE_SPELL = 10;

    // Game
    constexpr int DEFAULT_FIELD_WIDTH = 12;
    constexpr int DEFAULT_FIELD_HEIGHT = 12;
    constexpr int DEFAULT_PLAYER_X = 0;
    constexpr int DEFAULT_PLAYER_Y = 0;
    constexpr int DEFAULT_START_TURN = 1;
    constexpr int MELEE_DAMAGE_PER_UPDATE = 10;
    constexpr int RANGED_DAMAGE_PER_UPDATE = 10;
    constexpr int ATTACK_RANGE_PER_UPDATE = 1;

    // GameController
    constexpr int MAX_UPGRADES_PER_LEVEL = 2;
}

#endif