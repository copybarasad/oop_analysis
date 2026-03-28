#include "LoggerSystem.h"
#include "ConsoleLogger.h"
#include <sstream>
#include <variant>

namespace Game {

LoggerSystem::LoggerSystem(std::unique_ptr<ILogger> logger) 
    : loggerStrategy(std::move(logger)) {}

void LoggerSystem::onNotify(const Event& event) {
    std::string message = formatEvent(event);
    if (!message.empty() && loggerStrategy) {
        loggerStrategy->log(message);
    }
}

std::string LoggerSystem::formatEvent(const Event& event) const {
    std::stringstream ss;
    
    switch (event.type) {
        case EventType::LevelStarted:
            ss << "Level " << std::get<int>(event.data) << " started.";
            break;
        case EventType::TurnStarted:
            ss << "Turn " << std::get<int>(event.data) << " begins.";
            break;
        case EventType::PlayerMoved: {
            auto data = std::get<PositionData>(event.data);
            ss << "Player moved from (" << data.from.row << "," << data.from.column 
               << ") to (" << data.to.row << "," << data.to.column << ").";
            break;
        }
        case EventType::PlayerAttacked: {
            auto data = std::get<DamageData>(event.data);
            ss << "Player attacked " << data.target << " at (" << data.location.row << "," << data.location.column
               << ") dealing " << data.damage << " damage.";
            break;
        }
        case EventType::PlayerTookDamage: {
            auto data = std::get<DamageData>(event.data);
            ss << "Player took " << data.damage << " damage from " << data.dealer << " at ("
               << data.location.row << "," << data.location.column << ").";
            break;
        }
        case EventType::PlayerCastedSpell: {
            auto data = std::get<SpellCastData>(event.data);
            ss << "Player casted spell '" << data.spellName << "' at target ("
               << data.target.row << "," << data.target.column << ").";
            break;
        }
        case EventType::PlayerGainedSpell:
            ss << "Player received a new spell: " << std::get<std::string>(event.data);
            break;
        case EventType::EnemyDefeated: {
            auto pos = std::get<Utils::Point>(event.data);
            ss << "Player defeated an enemy at (" << pos.row << "," << pos.column << ").";
            break;
        }
        case EventType::LevelWon:
            ss << "Player won Level " << std::get<int>(event.data) << ".";
            break;
        case EventType::LevelLost:
            ss << "Player was defeated.";
            break;
        case EventType::GameSaved:
            ss << "Game state saved.";
            break;
        case EventType::GenericMessage:
            ss << std::get<std::string>(event.data);
            break;
        default:
            return "";
    }
    
    return ss.str();
}

}