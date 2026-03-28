#include "EventLogger.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

static std::string GetTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}

std::string ConsoleLogger::FormatEvent(const GameEvent& event) const {
    std::ostringstream oss;
    oss << "[" << GetTimestamp() << "] " << event.description;
    
    switch (event.type) {
        case GameEventType::PLAYER_MOVED:
            oss << " на (" << event.x << ", " << event.y << ")";
            break;
        case GameEventType::PLAYER_ATTACKED:
            oss << " " << event.entity_name << ", урон: " << event.damage;
            break;
        case GameEventType::PLAYER_DAMAGED:
            oss << ": " << event.damage << ", осталось HP: " << event.health;
            break;
        case GameEventType::ENEMY_MOVED:
            oss << " " << event.entity_name << " на (" << event.x << ", " << event.y << ")";
            break;
        case GameEventType::ENEMY_DAMAGED:
            oss << " " << event.entity_name << ": " << event.damage << ", осталось HP: " << event.health;
            break;
        case GameEventType::ENEMY_KILLED:
            oss << ": " << event.entity_name;
            break;
        case GameEventType::BUILDING_DAMAGED:
            oss << " на (" << event.x << ", " << event.y << "): " << event.damage << ", осталось HP: " << event.health;
            break;
        case GameEventType::BUILDING_DESTROYED:
            oss << " на (" << event.x << ", " << event.y << ")";
            break;
        case GameEventType::SPELL_RECEIVED:
            oss << ": " << event.spell_name;
            break;
        case GameEventType::SPELL_USED:
            oss << " " << event.spell_name << " на (" << event.x << ", " << event.y << ")";
            break;
        case GameEventType::TRAP_TRIGGERED:
            oss << " на " << event.entity_name << ", урон: " << event.damage;
            break;
        case GameEventType::LEVEL_STARTED:
        case GameEventType::LEVEL_COMPLETED:
            oss << " " << event.level;
            break;
        case GameEventType::GAME_SAVED:
        case GameEventType::GAME_LOADED:
            oss << ": " << event.entity_name;
            break;
        default:
            break;
    }
    
    return oss.str();
}

void ConsoleLogger::Log(const GameEvent& event) {
    std::cout << "[LOG] " << FormatEvent(event) << "\n";
}

FileLogger::FileLogger(const std::string& filename) {
    file_.open(filename, std::ios::app);
    if (!file_.is_open()) {
        std::cerr << "Не удалось открыть файл лога: " << filename << "\n";
    } else {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        file_ << "\n========================================\n";
        file_ << "=== Новая сессия: " << oss.str() << " ===\n";
        file_ << "========================================\n";
        file_.flush();
    }
}

FileLogger::~FileLogger() {
    if (file_.is_open()) {
        file_.close();
    }
}

std::string FileLogger::FormatEvent(const GameEvent& event) const {
    std::ostringstream oss;
    oss << "[" << GetTimestamp() << "] " << event.description;
    
    switch (event.type) {
        case GameEventType::PLAYER_MOVED:
            oss << " на (" << event.x << ", " << event.y << ")";
            break;
        case GameEventType::PLAYER_ATTACKED:
            oss << " " << event.entity_name << ", урон: " << event.damage;
            break;
        case GameEventType::PLAYER_DAMAGED:
            oss << ": " << event.damage << ", осталось HP: " << event.health;
            break;
        case GameEventType::ENEMY_MOVED:
            oss << " " << event.entity_name << " на (" << event.x << ", " << event.y << ")";
            break;
        case GameEventType::ENEMY_DAMAGED:
            oss << " " << event.entity_name << ": " << event.damage << ", осталось HP: " << event.health;
            break;
        case GameEventType::ENEMY_KILLED:
            oss << ": " << event.entity_name;
            break;
        case GameEventType::BUILDING_DAMAGED:
            oss << " на (" << event.x << ", " << event.y << "): " << event.damage << ", осталось HP: " << event.health;
            break;
        case GameEventType::BUILDING_DESTROYED:
            oss << " на (" << event.x << ", " << event.y << ")";
            break;
        case GameEventType::SPELL_RECEIVED:
            oss << ": " << event.spell_name;
            break;
        case GameEventType::SPELL_USED:
            oss << " " << event.spell_name << " на (" << event.x << ", " << event.y << ")";
            break;
        case GameEventType::TRAP_TRIGGERED:
            oss << " на " << event.entity_name << ", урон: " << event.damage;
            break;
        case GameEventType::LEVEL_STARTED:
        case GameEventType::LEVEL_COMPLETED:
            oss << " " << event.level;
            break;
        case GameEventType::GAME_SAVED:
        case GameEventType::GAME_LOADED:
            oss << ": " << event.entity_name;
            break;
        default:
            break;
    }
    
    return oss.str();
}

void FileLogger::Log(const GameEvent& event) {
    if (file_.is_open()) {
        file_ << FormatEvent(event) << "\n";
        file_.flush();
    }
}

std::unique_ptr<IEventLogger> LoggerFactory::Create(LoggerType type, const std::string& filename) {
    switch (type) {
        case LoggerType::CONSOLE:
            return std::make_unique<ConsoleLogger>();
        case LoggerType::FILE:
            return std::make_unique<FileLogger>(filename);
        case LoggerType::NONE:
        default:
            return nullptr;
    }
}

LoggerFactory::LoggerType LoggerFactory::ParseArgs(int argc, char* argv[], std::string& out_filename) {
    out_filename = "game.log";
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--log-console" || arg == "-lc") {
            return LoggerType::CONSOLE;
        }
        else if (arg == "--log-file" || arg == "-lf") {
            if (i + 1 < argc) {
                out_filename = argv[i + 1];
            }
            return LoggerType::FILE;
        }
        else if (arg == "--no-log") {
            return LoggerType::NONE;
        }
    }
    
    return LoggerType::NONE;
}
