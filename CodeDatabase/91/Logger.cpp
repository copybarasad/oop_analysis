#include "Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>
#include <utility>

Logger::Logger(OutputType type, const std::string& filepath)
    : output_type_(type), log_file_path_(filepath), output_stream_(&std::cout) {
    if (type == OutputType::FILE) {
        if (!filepath.empty()) {
            file_stream_ = std::make_unique<std::ofstream>(filepath, std::ios::trunc);
            if (file_stream_->is_open()) {
                output_stream_ = file_stream_.get();
            } else {
                std::cerr << "Warning: Could not open log file " << filepath << ". Logging to console." << std::endl;
                output_type_ = OutputType::CONSOLE;
                output_stream_ = &std::cout;
            }
        } else {
            std::cerr << "Warning: Log file path is empty. Logging to console." << std::endl;
            output_type_ = OutputType::CONSOLE;
            output_stream_ = &std::cout;
        }
    }
}

Logger::~Logger(){}

std::string Logger::formatMessage(const LogEvent& event) {
    auto now = std::chrono::system_clock::now();
    std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");

    std::string type_str;
    switch (event.type) {
        case LogEventType::PLAYER_MOVE: type_str = "PLAYER_MOVE"; break;
        case LogEventType::PLAYER_DAMAGE_TAKEN: type_str = "PLAYER_DAMAGE_TAKEN"; break;
        case LogEventType::PLAYER_DAMAGE_DEALT: type_str = "Player_DAMAGE_DEALT"; break;
        case LogEventType::ENEMY_DAMAGE_TAKEN: type_str = "ENEMY_DAMAGE_TAKEN"; break;
        case LogEventType::ENEMY_DAMAGE_DEALT: type_str = "ENEMY_DAMAGE_DEALT"; break;
        case LogEventType::SPELL_CAST: type_str = "SPELL_CAST"; break;
        case LogEventType::SPELL_LEARNED: type_str = "SPELL_LEARNED"; break;
        case LogEventType::ITEM_ACQUIRED: type_str = "ITEM_ACQUIRED"; break;
        case LogEventType::GAME_SAVE: type_str = "GAME_SAVE"; break;
        case LogEventType::GAME_LOAD: type_str = "GAME_LOAD"; break;
        case LogEventType::DEATH_MESSAGE: type_str = "DEATH_MESSAGE"; break;
        case LogEventType::ENEMY_SPAWNED: type_str = "ENEMY_SPAWNED"; break;
        case LogEventType::ENEMY_MOVE: type_str = "ENEMY_MOVE"; break;
        case LogEventType::COMBAT_TYPE_SWITCH: type_str = "COMBAT_TYPE_SWITCH"; break;
        case LogEventType::TOWER_DAMAGE_DEALT: type_str = "TOWER_DAMAGE_DEALT"; break;
        case LogEventType::TOWER_DAMAGE_TAKEN: type_str = "TOWER_DAMAGE_TAKEN"; break;
        case LogEventType::AREA_SPELL_DAMAGE: type_str = "AREA_SPELL_DAMAGE"; break;
        case LogEventType::NEW_LEVEL_STARTED: type_str = "NEW_LEVEL_STARTED"; break;
        default: type_str = "UNKNOWN";
    }

    return "[" + ss.str() + "] [" + type_str + "] " + event.message;
}

void Logger::log(const LogEvent& event) {
    if (output_stream_) {
        *output_stream_ << formatMessage(event) << std::endl;
        output_stream_->flush();
    }
}

void Logger::logPlayerMove(int from_x, int from_y, int to_x, int to_y) {
    log(LogEvent(LogEventType::PLAYER_MOVE,
                 "Player moved from (" + std::to_string(from_x) + ", " + std::to_string(from_y) + ") to (" +
                 std::to_string(to_x) + ", " + std::to_string(to_y) + ")."));
}

void Logger::logDamage(const std::string& source, const std::string& target, int amount, const std::string& type) {
    LogEventType log_type = LogEventType::PLAYER_DAMAGE_TAKEN;
    if (type == "PLAYER_DAMAGE_TAKEN") log_type = LogEventType::PLAYER_DAMAGE_TAKEN;
    else if (type == "PLAYER_DAMAGE_DEALT") log_type = LogEventType::PLAYER_DAMAGE_DEALT;
    else if (type == "ENEMY_DAMAGE_TAKEN") log_type = LogEventType::ENEMY_DAMAGE_TAKEN;
    else if (type == "ENEMY_DAMAGE_DEALT") log_type = LogEventType::ENEMY_DAMAGE_DEALT;
    log(LogEvent(log_type, source + " dealt " + std::to_string(amount) + " damage to " + target + "."));
}

void Logger::logSpellCast(const std::string& playerName, const std::string& spellName, int x, int y) {
    log(LogEvent(LogEventType::SPELL_CAST,
                 playerName + " cast " + spellName + " at (" + std::to_string(x) + ", " + std::to_string(y) + ")."));
}

void Logger::logSpellLearned(const std::string& playerName, const std::string& spellName) {
    log(LogEvent(LogEventType::SPELL_LEARNED,
                 playerName + " learned new spell: " + spellName + "."));
}

void Logger::logEnemySpawned(const std::string& enemyName, int x, int y) {
    log(LogEvent(LogEventType::ENEMY_SPAWNED,
                 "Enemy " + enemyName + " spawned at (" + std::to_string(x) + ", " + std::to_string(y) + ")."));
}

void Logger::logEnemyMove(int from_x, int from_y, int to_x, int to_y) {
    log(LogEvent(LogEventType::ENEMY_MOVE,
                 "Enemy moved from (" + std::to_string(from_x) + ", " + std::to_string(from_y) + ") to (" +
                 std::to_string(to_x) + ", " + std::to_string(to_y) + ")."));
}

void Logger::logCombatTypeSwitch(const std::string& playerName, const std::string& newCombatType) {
    log(LogEvent(LogEventType::COMBAT_TYPE_SWITCH,
                 playerName + " switched to " + newCombatType + " combat."));
}

void Logger::logTowerDamageDealt(const std::string& towerName, const std::string& target, int amount) {
    log(LogEvent(LogEventType::TOWER_DAMAGE_DEALT,
                 towerName + " dealt " + std::to_string(amount) + " damage to " + target + "."));
}

void Logger::logTowerDamageTaken(const std::string& towerName, const std::string& source, int amount) {
    log(LogEvent(LogEventType::TOWER_DAMAGE_TAKEN,
                 towerName + " took " + std::to_string(amount) + " damage from " + source + "."));
}

void Logger::logAreaSpellDamage(const std::string& spellName, int centerX, int centerY, int areaSizeX, int areaSizeY, const std::vector<std::pair<int, int>>& affectedPositions, int totalDamageDealt) {
    std::string positions_str = "[";
    for (size_t i = 0; i < affectedPositions.size(); ++i) {
        positions_str += "(" + std::to_string(affectedPositions[i].first) + "," + std::to_string(affectedPositions[i].second) + ")";
        if (i < affectedPositions.size() - 1) positions_str += ", ";
    }
    positions_str += "]";

    log(LogEvent(LogEventType::AREA_SPELL_DAMAGE,
                 spellName + " hit area centered at (" + std::to_string(centerX) + "," + std::to_string(centerY) + ") size (" + std::to_string(areaSizeX) + "x" + std::to_string(areaSizeY) + "). Affected positions: " + positions_str + ". Total damage: " + std::to_string(totalDamageDealt) + "."));
}

void Logger::logNewLevelStarted(int levelNumber, int fieldWidth, int fieldHeight) {
    log(LogEvent(LogEventType::NEW_LEVEL_STARTED,
                 "Starting Level " + std::to_string(levelNumber) + ". Field size: " + std::to_string(fieldWidth) + "x" + std::to_string(fieldHeight) + "."));
}

void Logger::setOutput(OutputType type, const std::string& filepath) {
    file_stream_.reset();

    output_type_ = type;
    log_file_path_ = filepath;

    if (type == OutputType::FILE) {
        if (!filepath.empty()) {
            file_stream_ = std::make_unique<std::ofstream>(filepath, std::ios::app);
            if (file_stream_->is_open()) {
                output_stream_ = file_stream_.get();
            } else {
                std::cerr << "Warning: Could not open log file " << filepath << ". Logging to console." << std::endl;
                output_type_ = OutputType::CONSOLE;
                output_stream_ = &std::cout;
            }
        } else {
            std::cerr << "Warning: Log file path is empty. Logging to console." << std::endl;
            output_type_ = OutputType::CONSOLE;
            output_stream_ = &std::cout;
        }
    } else { // Console
        output_stream_ = &std::cout;
    }
}

void Logger::logGenericMessage(const std::string& message) {
    log(LogEvent(LogEventType::DEATH_MESSAGE, message));
}

void Logger::logEnemyDefeated(const std::string& enemyName, const std::string& killerName, int x, int y) {
    log(LogEvent(LogEventType::ENEMY_DEFEATED, killerName + " defeated " + enemyName + " at (" + std::to_string(x) + ", " + std::to_string(y) + ")."));
}

void Logger::logPlayerDefeated(const std::string& playerName, const std::string& killerName, int x, int y) {
    log(LogEvent(LogEventType::PLAYER_DEFEATED, killerName + " defeated " + playerName + " at (" + std::to_string(x) + ", " + std::to_string(y) + ")."));
}