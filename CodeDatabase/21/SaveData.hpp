#pragma once
#include <vector>

struct AttributesSaveData {
    int intelligence;
    int dexterity;
    int strength;
};


struct AttackSaveData {
    int attack;
};


struct EquipmentSaveData {
    int currentWeapon; // 0 - MELEE 1 - RANGE
    int meleeWeaponMulti;
    int rangeWeaponMulti;
};


struct SpellSaveData {
    int countOfItem;
    int powerOfSpell;
    int baseDamage;
    int baseDistance;
    int trapLevel;
    int trapDamage;
};


struct HandSaveData {
    int itemInHand; //0-3 Area, Direct, Update, Trap
    int powerUp;
    int maxSize;
    int currentSize;
    struct SpellSaveData areaSpell;
    struct SpellSaveData directSpell;
    struct SpellSaveData updateSpell;
    struct SpellSaveData trapSpell;
};


struct StatsSaveData {
    long long prevLevelUpExperience;
    long long currentExperience;
    long long levelUpExperience;
    int level;
    bool levelIncreased;
};


struct HealthSaveData {
    int currentHealth;
    int maxHealth;
};


struct StateSaveData {
    int durationOfState;
    int damage;
    char stateSymbol;
};


struct CellStateSaveData {
    int avaible;
    int haveConstState;
    int haveTempState;
    struct StateSaveData constState;
    struct StateSaveData tempState;
};


struct CellSaveData {
    int index;
    int X;
    int Y;
    struct CellStateSaveData stateData;
};


struct BarrackSaveData {
    int spawnPeriod;
    int barracksLevel;
    int counter;
    int barrackIndex;
    struct HealthSaveData barracksHealth;
};


struct TowerSaveData {
    int attackPeriod;
    int attackCooldown;
    int towerlevel;
    int towerIndex;
    struct HealthSaveData towerHealth;
    struct SpellSaveData towerSpell;
};


struct EnemySaveData {
    struct AttackSaveData enemyAttack;
    struct HealthSaveData enemyHealth;
    int enemyLevel;
    int enemyIndex;
};


struct PlayerSaveData {
    struct AttackSaveData playerAttack;
    struct AttributesSaveData playerAttributes;
    struct EquipmentSaveData playerEquipment;
    struct HandSaveData playerHand;
    struct HealthSaveData playerHealth;
    struct StatsSaveData playerStats;
    int playerIndex;
    bool slowed;
};


struct EntitySaveData {
    struct PlayerSaveData playerData;
    std::vector<struct EnemySaveData> enemyData;
    struct BarrackSaveData barrackData;
    struct TowerSaveData towerData;
};


struct FieldSaveData {
    int widthField;
    int heightField;
    int gameLevel;
    int gameTurn;
    struct EntitySaveData entityData;
    std::vector<struct CellSaveData> cellsData;
};


struct SaveData {
    int gameID;
    struct FieldSaveData fieldData;
};
