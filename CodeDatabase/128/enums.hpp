#pragma once

enum class CellType {
    kEmpty,
    kPlayer,
    kEnemy,
    kWall,
    kBuilding,
    kSlow,
    kTrap, 
    kAlly,
    kTower
};

enum class LogOutput {
    None,
    Console,
    File,
    Both  
};

enum class RewardType {
    KillEnemy,
    DestroyStructure
};

enum class GameStateType {
    Menu,
    Upgrade,
    PlayProcess,
    LevelComplete,
    LevelTransition,
    Quit
};

enum class SpellType {
    kDirectDamage,
    kAreaDamage,
    kSummon,
    kTrapped,
    kEnhancement
};

enum class EntityType {
    Player,
    Enemy,
    EnemyTower,
    EnemyBuilding,
    Ally, 
    Summon,   
    Trap    
};

enum class CommandType {
    None,
    Quit,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    ToggleMode,
    UseSpell,
    Save,
    Load
};