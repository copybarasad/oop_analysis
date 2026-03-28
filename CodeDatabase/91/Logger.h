#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <vector>

enum class LogEventType {
    PLAYER_MOVE,
    PLAYER_DAMAGE_TAKEN,
    PLAYER_DAMAGE_DEALT,
    ENEMY_DAMAGE_TAKEN,
    ENEMY_DAMAGE_DEALT,
    SPELL_CAST,
    SPELL_LEARNED,
    ITEM_ACQUIRED,
    GAME_SAVE,
    GAME_LOAD,
    ENEMY_SPAWNED,
    ENEMY_MOVE,
    COMBAT_TYPE_SWITCH,
    TOWER_DAMAGE_DEALT,
    TOWER_DAMAGE_TAKEN,
    AREA_SPELL_DAMAGE,
    NEW_LEVEL_STARTED,
    DEATH_MESSAGE,
    ENEMY_DEFEATED,
    PLAYER_DEFEATED,
};

struct LogEvent {
    LogEventType type;
    std::string message;

    LogEvent(LogEventType t, const std::string& msg) : type(t), message(msg) {}
};

class Logger {
    public:
        enum class OutputType {
        CONSOLE,
        FILE
    };

    private:
        OutputType output_type_;
        std::unique_ptr<std::ofstream> file_stream_;
        std::string log_file_path_;
        std::ostream* output_stream_;

        std::string formatMessage(const LogEvent& event);

    public:
        Logger(OutputType type, const std::string& filepath = "");
        ~Logger();

        void log(const LogEvent& event);

        void logPlayerMove(int from_x, int from_y, int to_x, int to_y);
        void logDamage(const std::string& source, const std::string& target, int amount, const std::string& type = "GENERAL_DAMAGE");
        void logSpellCast(const std::string& playerName, const std::string& spellName, int x, int y);
        void logSpellLearned(const std::string& playerName, const std::string& spellName);
        void logEnemySpawned(const std::string& enemyName, int x, int y);
        void logEnemyMove(int from_x, int from_y, int to_x, int to_y);
        void logCombatTypeSwitch(const std::string& playerName, const std::string& newCombatType);
        void logTowerDamageDealt(const std::string& towerName, const std::string& target, int amount);
        void logTowerDamageTaken(const std::string& towerName, const std::string& source, int amount);
        void logAreaSpellDamage(const std::string& spellName, int centerX, int centerY, int areaSizeX, int areaSizeY, const std::vector<std::pair<int, int>>& affectedPositions, int totalDamageDealt);
        void logNewLevelStarted(int levelNumber, int fieldWidth, int fieldHeight);
        void logGenericMessage(const std::string& message);
        void logEnemyDefeated(const std::string& enemyName, const std::string& killerName, int x, int y);
        void logPlayerDefeated(const std::string& playerName, const std::string& killerName, int x, int y);

        void setOutput(OutputType type, const std::string& filepath = "");
};

#endif