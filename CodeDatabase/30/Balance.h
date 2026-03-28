#pragma once

struct Balance {
    // Уровень
    static inline int fieldRows(int level) { 
        int val = 10 + (level - 1) * 2; 
        return val > 25 ? 25 : val;
    }
    static inline int fieldCols(int level) { 
        int val = 10 + (level - 1) * 2; 
        return val > 25 ? 25 : val;
    }

    // Игрок
    static constexpr int PlayerBaseHP = 100;
    static constexpr int PlayerBaseDmg = 15;
    static constexpr int PlayerHandSize = 5;
    static constexpr int PlayerMeleeRange = 1;
    static constexpr int PlayerRangedRange = 4;

    // Враги
    static inline int EnemyHP(int level) { return 30 + (level - 1) * 5; }
    static inline int EnemyDmg(int level) { return 8 + (level - 1) * 1; }
    static inline int EnemyCount(int level) { return 3 + (level - 1); }

    // Здания (спавнеры)
    static constexpr int BuildingBaseHP = 1;
    static inline int BuildingSpawnInterval(int level) { 
        // Интервал спавна: уменьшается с уровнем, но не меньше 3
        int val = 8 - (level - 1) / 2; 
        return val < 3 ? 3 : val; 
    }
    static inline int BuildingCount(int level) { return 1 + (level / 3); }

    // Башни
    static inline int TowerHP(int level) { return 40 + (level - 1) * 10; }
    static inline int TowerDmg(int level) { return 10 + (level - 1) * 2; }
    static constexpr int TowerRange = 4;
    static constexpr int TowerCooldown = 2;
    static inline int TowerCount(int level) { return 1 + (level / 2); }

    // Союзники
    static constexpr int AllyHP = 40;
    static constexpr int AllyDmg = 10;

    // Заклинания
    static constexpr int SpellDirectDmg = 40;
    static constexpr int SpellDirectRange = 5;
    
    static constexpr int SpellAreaDmg = 25;
    static constexpr int SpellAreaRange = 4;
    
    static constexpr int SpellTrapDmg = 50;
};
