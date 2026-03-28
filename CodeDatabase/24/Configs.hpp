#pragma once
#include <cstdint>

// Структура для конфигурации уровня
struct LevelConfig {
    int fieldWidth;
    int fieldHeight;
    int enemyCount;
    int enemyHealth;
    int enemyDamage;
};

// Пространство имен для конфигурационных параметров
namespace Config {
    // Ограничения
    namespace Constraints {
        constexpr int MIN_FIELD_SIZE = 10;
        constexpr int MAX_FIELD_SIZE = 25;
        constexpr int MIN_HEALTH = 1;
        constexpr int MAX_HEALTH = 100;
        constexpr int MIN_DAMAGE = 1;
        constexpr int MAX_DAMAGE = 100;
        constexpr int MIN_HAND_SIZE = 0;
        constexpr int MAX_HAND_SIZE = 2;

        // Функции валидации
        [[maybe_unused]] static bool validateHandSize(int size) {
            return size >= MIN_HAND_SIZE && size <= MAX_HAND_SIZE;
        }
        
        [[maybe_unused]] static bool validateHealth(int health) {
            return health >= MIN_HEALTH && health <= MAX_HEALTH;
        }
        
        [[maybe_unused]] static bool validateDamage(int damage) {
            return damage >= MIN_DAMAGE && damage <= MAX_DAMAGE;
        }
        
        [[maybe_unused]] static bool validateFieldSize(int size) {
            return size >= MIN_FIELD_SIZE && size <= MAX_FIELD_SIZE;
        }
        
        [[maybe_unused]] static bool validateAllConstraints() {
            return MIN_HEALTH <= MAX_HEALTH && 
                   MIN_DAMAGE <= MAX_DAMAGE && 
                   MIN_FIELD_SIZE <= MAX_FIELD_SIZE;
        }
    }
    
    // Параметры
    namespace Settings {
        constexpr int PLAYER_MELEE_ATTACK_RANGE = 1;
        constexpr int PLAYER_RANGED_ATTACK_RANGE = 3;
        constexpr int PLAYER_SCORE_PER_KILL = 10;
        constexpr int RANGED_DAMAGE_DIVIDER = 2;

        constexpr int SPELL_DIRECT_DAMAGE = 20;
        constexpr int SPELL_DIRECT_RANGE = 3;

        constexpr int SPELL_AREA_DAMAGE = 10;
        constexpr int SPELL_AREA_RANGE = 4;

        constexpr int SPELL_AWARD_THRESHOLD = 20;

        constexpr int DEFAULT_HAND_SIZE = 2;

        constexpr int DEFAULT_FIELD_WIDTH = 25;
        constexpr int DEFAULT_FIELD_HEIGHT = 10;
        
        constexpr int PLAYER_START_HEALTH = 100;
        constexpr int PLAYER_START_DAMAGE = 10;
        
        constexpr int ENEMY_START_HEALTH = 30;
        constexpr int ENEMY_START_DAMAGE = 5;
        
        constexpr int PLAYER_START_X = 0;
        constexpr int PLAYER_START_Y = 0;

        constexpr int MAX_LEVELS = 3;
        
        // Параметры сохранения
        constexpr const char* SAVE_DIRECTORY = "saves/";
        constexpr const char* SAVE_EXTENSION = ".sav";
    }

    // Конфигурации уровней
    namespace Levels {
        constexpr LevelConfig LEVEL_1 = {
            15,  // width
            10,  // height
            2,   // enemyCount
            30,  // enemyHealth
            5    // enemyDamage
        };

        constexpr LevelConfig LEVEL_2 = {
            20,  // width
            10,  // height
            3,   // enemyCount
            40,  // enemyHealth
            8    // enemyDamage
        };

        constexpr LevelConfig LEVEL_3 = {
            25,  // width
            10,  // height
            4,   // enemyCount
            50,  // enemyHealth
            12   // enemyDamage
        };
    }
    
    // Отображение - символы для консоли
    namespace Graphics {
        constexpr char PLAYER_SYMBOL = 'P';
        constexpr char ENEMY_SYMBOL = 'E';
        constexpr char EMPTY_CELL_SYMBOL = '.';
        constexpr char OBSTACLE_SYMBOL = '#';
    }
}
