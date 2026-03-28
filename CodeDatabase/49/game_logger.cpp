#include "game_logger.h"

GameLogger::GameLogger(LogOutput /*output*/, const std::string& filepath)
{
    file_stream.open(filepath, std::ios::app);
}

GameLogger::~GameLogger()
{
    if (file_stream.is_open())
    {
        file_stream.close();
    }
}

std::string GameLogger::get_timestamp() const
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return oss.str();
}

std::string GameLogger::event_type_to_string(GameEventType type) const
{
    switch (type)
    {
        case GameEventType::PLAYER_MOVED:
            return "PLAYER_MOVED";
        case GameEventType::PLAYER_ATTACKED:
            return "PLAYER_ATTACKED";
        case GameEventType::PLAYER_DAMAGED:
            return "PLAYER_DAMAGED";
        case GameEventType::PLAYER_MODE_CHANGED:
            return "PLAYER_MODE_CHANGED";
        case GameEventType::SPELL_CAST:
            return "SPELL_CAST";
        case GameEventType::ENEMY_MOVED:
            return "ENEMY_MOVED";
        case GameEventType::ENEMY_DAMAGED:
            return "ENEMY_DAMAGED";
        case GameEventType::BUILDING_DAMAGED:
            return "BUILDING_DAMAGED";
        case GameEventType::BUILDING_SPAWNED_ENEMY:
            return "BUILDING_SPAWNED_ENEMY";
        case GameEventType::SPELL_RECEIVED:
            return "SPELL_RECEIVED";
        case GameEventType::LEVEL_CHANGED:
            return "LEVEL_CHANGED";
        default:
            return "UNKNOWN";
    }
}

void GameLogger::write_log(const std::string& message)
{
    if (file_stream.is_open())
    {
        file_stream << message << std::endl;
    }
}

void GameLogger::on_event(const GameEvent& event)
{
    std::ostringstream oss;
    oss << "[" << get_timestamp() << "] "
        << "[" << event_type_to_string(event.type) << "] "
        << event.description;
    
    write_log(oss.str());
}