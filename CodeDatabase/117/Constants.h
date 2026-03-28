#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Константы для LogSystem
const std::string DEFAULT_LOGFILE_NAME = "logs.log";

// Константы для системы сохранений и InputController
const std::string KEYBOARD_SETTINGS_FILE_PATH = "./system files/KeyboardSettings";
const std::string FIRST_LEVEL_NAME = "Level 1";
const std::string LEVEL_PATH = "./levels";
const std::string SAVES_PATH = "./saves";
const std::string SAVE_LEVEL_PATH = "./system files/levelSave";
const int MAX_LEVEL_NUM = 3;
const int MAX_SLOT_NUM = 5;

// Константы для Field
const int DEFAULT_FIELD_SIZE = 10;
const int MIN_FIELD_SIZE = 10;
const int MAX_FIELD_SIZE = 25;

// Константы для BehaviourSystem
const int INF = 1000000;
const int EXP_FOR_KILL_BY_ALLY = 5;


// Константы для Character и его наследников

// Константы для Player
const int DEFAULT_MELEE_DAMAGE = 40;
const int DEFAULT_RANGE_DAMAGE = 20;
const int DEFAULT_START_EXP = 0;
const int DISTANCE_FOR_MELEE = 1;
const int DISTANCE_FOR_RANGE = 5;
const int EXP_FOR_KILL = 10;

// Константы для Enemy
const int DEFAULT_DAMAGE = 20;

// Константы для Ally
const int DEFAULT_ALLY_DAMAGE = 20;

// Константы для EnemyBuilding
const int DEFAULT_MAX_SPAWN_TURNS = 3;

// Константы для EnemyTower
const int DEFAULT_TOWER_DAMAGE = 10;
const int DEFAULT_TOWER_RANGE = 3;

// Константы для Trap
const int DEFAULT_TRAP_DAMAGE = 40;
const int TRAP_HEALTH = 1;

// Константы для Character
const int DEFAULT_MAX_HEALTH = 100;


// Константы для заклинаний и так далее

// Константы для Hand
const int DEFAULT_HAND_SIZE = 5;

// Константы для TrapSpell
const int TRAP_SPELL_COST = 5;

// Константы для DirDMGSpell
const int DEFAULT_DIR_SPELL_DAMAGE = 30;
const int DEFAULT_DIR_SPELL_RANGE = 5;
const int DIR_SPELL_COST = 5;

// Константы для AreaDMGSpell
const int DEFAULT_AREA_SPELL_DAMAGE = 15;
const int DEFAULT_AREA_SPELL_RANGE = 5;
const int DEFAULT_AREA_SPELL_LENSIDE = 2;
const int AREA_SPELL_COST = 5;

// Константы для UpgradeSpell
const int UPGRADE_SPELL_COST = 3;
const int DEFAULT_UPGRADE_POWER = 1;

// Константы для SummonSpell
const int DEFAULT_ALLIES_NUM = 1;
const int SUMMON_SPELL_COST = 10;

#endif