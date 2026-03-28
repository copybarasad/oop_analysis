#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include <string>
#include <fstream>

enum class LogOutput {
    CONSOLE,
    FILE,
    BOTH
};

// Список всех возможных сообщений в игре
enum class LogMsg {
    //Поле
    FIELD_LOAD_ERROR,      
    FIELD_SAVE_ERROR,      
    FIELD_INVALID_SIZE,     


    //ИГра
    GAME_STARTED,
    GAME_RESTARTED,
    GAME_OVER,
    LEVEL_COMPLETE,
    LEVEL_START,
    EXITING,
    
    // Действия игрока
    HIT_WALL,
    CELL_OCCUPIED,
    STEPPED_TRAP,
    ATTACK_MELEE,
    ATTACK_RANGED,
    MOVED_SLOWED,
    SWITCH_MODE,
    
    // Заклинания и магазин
    SPELL_CAST_CANCEL,
    SPELL_NO_SPELLS,
    SPELL_INVALID_INDEX,
    SPELL_BOUGHT, 
    SPELL_HAND_FULL,
    SPELL_NOT_ENOUGH_SCORE,
    SPELL_FIREBALL_HIT,
    SPELL_FIREBALL_WALL,
    SPELL_ZAP_HIT,
    SPELL_TRAP_PLACED,
    SPELL_TRAP_FAIL,
    SPELL_SUMMON, 
    SPELL_SUMMON_FAIL,
    SPELL_BUFF,
    SPELL_FAIL,
    // События мира
    ENEMY_HIT_PLAYER, 
    ENEMY_STEPPED_TRAP,
    TOWER_HIT_PLAYER,
    ENTITY_DESTROYED,
    
    // Сохранение/Загрузка
    SAVE_SUCCESS,
    SAVE_FAIL, 
    LOAD_SUCCESS,
    LOAD_FAIL, 
    
    // Прокачка
    UPGRADE_HEALTH,
    UPGRADE_DAMAGE,
    FATIGUE_LOST_SPELLS 
};

class EventLogger {
public:
    EventLogger(LogOutput outputType, const std::string& filename = "game_log.txt");
    ~EventLogger();
    void log(LogMsg msg);
    void log(LogMsg msg, int value);
    void log(LogMsg msg, const std::string& text);

private:
    LogOutput outputType;
    std::ofstream fileStream;
    void writeString(const std::string& message);
    std::string resolveMessage(LogMsg msg);
};
#endif