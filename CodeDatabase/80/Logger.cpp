#include "Logger.h"
#include <filesystem>

Logger::~Logger() {
    if (logFile.is_open()) {
        log(LogLevel::INFO, "Logger shutting down");
        logFile.close();
    }
    listeners.clear();
}

bool Logger::initialize(const std::string& filePath, LogLevel level, bool consoleOutput) {
    if (initialized) {
        return true; 
    }

    logFilePath = filePath;
    currentLevel = level;
    logToConsole = consoleOutput;

    try {
        std::filesystem::path path(filePath);
        if (path.has_parent_path()) {
            std::filesystem::create_directories(path.parent_path());
        }

        logFile.open(filePath, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "ERROR: Could not open log file: " << filePath << std::endl;
            return false;
        }

        initialized = true;
        logEvent(GameEventType::GAME_STARTED, "Logger initialized successfully");

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: Logger initialization failed: " << e.what() << std::endl;
        return false;
    }
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif

    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG: return "DEBUG";
    case LogLevel::INFO: return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERROR: return "ERROR";
    case LogLevel::CRITICAL: return "CRITICAL";
    default: return "UNKNOWN";
    }
}

std::string Logger::eventTypeToString(GameEventType type) {
    switch (type) {
    case GameEventType::PLAYER_MOVE: return "PLAYER_MOVE";
    case GameEventType::PLAYER_ATTACK: return "PLAYER_ATTACK";
    case GameEventType::PLAYER_USE_SPELL: return "PLAYER_USE_SPELL";
    case GameEventType::PLAYER_DAMAGE_TAKEN: return "PLAYER_DAMAGE_TAKEN";
    case GameEventType::PLAYER_HEALED: return "PLAYER_HEALED";
    case GameEventType::PLAYER_LEVEL_UP: return "PLAYER_LEVEL_UP";
    case GameEventType::PLAYER_SPELL_GAINED: return "PLAYER_SPELL_GAINED";
    case GameEventType::PLAYER_SPELL_USED: return "PLAYER_SPELL_USED";

    case GameEventType::ENEMY_SPAWNED: return "ENEMY_SPAWNED";
    case GameEventType::ENEMY_MOVED: return "ENEMY_MOVED";
    case GameEventType::ENEMY_ATTACK: return "ENEMY_ATTACK";
    case GameEventType::ENEMY_DAMAGE_TAKEN: return "ENEMY_DAMAGE_TAKEN";
    case GameEventType::ENEMY_DEFEATED: return "ENEMY_DEFEATED";

    case GameEventType::TOWER_SPAWN_ENEMY: return "TOWER_SPAWN_ENEMY";
    case GameEventType::TOWER_ATTACK: return "TOWER_ATTACK";
    case GameEventType::TOWER_DAMAGE_TAKEN: return "TOWER_DAMAGE_TAKEN";

    case GameEventType::GAME_STARTED: return "GAME_STARTED";
    case GameEventType::GAME_SAVED: return "GAME_SAVED";
    case GameEventType::GAME_LOADED: return "GAME_LOADED";
    case GameEventType::LEVEL_STARTED: return "LEVEL_STARTED";
    case GameEventType::LEVEL_COMPLETED: return "LEVEL_COMPLETED";
    case GameEventType::GAME_OVER: return "GAME_OVER";
    case GameEventType::VICTORY: return "VICTORY";

    case GameEventType::SPELL_CAST: return "SPELL_CAST";
    case GameEventType::SPELL_DAMAGE: return "SPELL_DAMAGE";
    case GameEventType::SPELL_AOE_DAMAGE: return "SPELL_AOE_DAMAGE";
    case GameEventType::SPELL_TRAP_ACTIVATED: return "SPELL_TRAP_ACTIVATED";

    default: return "UNKNOWN_EVENT";
    }
}

void Logger::writeToFile(const std::string& message) {
    if (!initialized || !logFile.is_open()) {
        return;
    }

    try {
        logFile << message << std::endl;
        logFile.flush();

        if (logToConsole) {
            std::cout << message << std::endl;
        }
    }
    catch (...) {
    }
}

void Logger::notifyListeners(GameEventType type, const std::string& description) {
    for (auto listener : listeners) {
        try {
            listener->onGameEvent(type, description);
        }
        catch (...) {
        }
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (!initialized || static_cast<int>(level) < static_cast<int>(currentLevel)) {
        return;
    }

    std::stringstream ss;
    ss << "[" << getTimestamp() << "] "
        << "[" << levelToString(level) << "] "
        << message;

    writeToFile(ss.str());
}

void Logger::logEvent(GameEventType type, const std::string& description,
    const std::unordered_map<std::string, std::string>& details) {
    if (!initialized) {
        return;
    }

    std::stringstream ss;
    ss << "[" << getTimestamp() << "] "
        << "[" << eventTypeToString(type) << "] "
        << description;

    if (!details.empty()) {
        ss << " [";
        bool first = true;
        for (const auto& [key, value] : details) {
            if (!first) ss << ", ";
            ss << key << "=" << value;
            first = false;
        }
        ss << "]";
    }

    writeToFile(ss.str());
    notifyListeners(type, description);
}

void Logger::addListener(IGameEventListener* listener) {
    listeners.push_back(listener);
}

void Logger::removeListener(IGameEventListener* listener) {
    for (auto it = listeners.begin(); it != listeners.end(); ++it) {
        if (*it == listener) {
            listeners.erase(it);
            break;
        }
    }
}

std::unordered_map<std::string, std::string> Logger::createDamageDetails(
    const std::string& attacker, const std::string& target, int damage) {
    return {
        {"attacker", attacker},
        {"target", target},
        {"damage", std::to_string(damage)}
    };
}

std::unordered_map<std::string, std::string> Logger::createMoveDetails(
    int fromX, int fromY, int toX, int toY) {
    return {
        {"from", "(" + std::to_string(fromX) + "," + std::to_string(fromY) + ")"},
        {"to", "(" + std::to_string(toX) + "," + std::to_string(toY) + ")"}
    };
}

std::unordered_map<std::string, std::string> Logger::createSpellDetails(
    const std::string& spellName, int damage, int range) {
    return {
        {"spell", spellName},
        {"damage", std::to_string(damage)},
        {"range", std::to_string(range)}
    };
}

std::unordered_map<std::string, std::string> Logger::createEntityDetails(
    const std::string& entityType, int x, int y, int health) {
    return {
        {"type", entityType},
        {"position", "(" + std::to_string(x) + "," + std::to_string(y) + ")"},
        {"health", std::to_string(health)}
    };
}