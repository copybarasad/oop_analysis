#include "logger.h"
#include "game_event.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

Logger::Logger(LogMode log_mode, const std::string& filename) : mode(log_mode) {
    if (mode == LogMode::File || mode == LogMode::Both) {
        log_file.open(filename, std::ios::out | std::ios::trunc);
    }
}

Logger::~Logger() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

void Logger::write(const std::string& message) {
    if (mode == LogMode::None) return;

    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::stringstream ss;
    ss << "[" << std::put_time(&tm, "%H:%M:%S") << "] " << message;
    std::string final_msg = ss.str();

    if (mode == LogMode::Console || mode == LogMode::Both) {
        std::cout << final_msg << std::endl;
    }
    if ((mode == LogMode::File || mode == LogMode::Both) && log_file.is_open()) {
        log_file << final_msg << std::endl;
    }
}

void Logger::onNotify(const GameEvent& event) {
    std::string msg_text;
    
    switch (event.type) {
        case EventType::CommandExecuted:
            msg_text = "[INPUT] Command: " + event.message;
            break;
            
        case EventType::StateChanged:
            msg_text = "[MOVE] " + event.message + " -> (" + std::to_string(event.val1) + ", " + std::to_string(event.val2) + ")";
            break;
            
        case EventType::DamageDealt:
            msg_text = "[ATTACK] " + event.message + " dealt " + std::to_string(event.val1) + " dmg";
            break;
            
        case EventType::DamageTaken:
            msg_text = "[HIT] " + event.message + " took " + std::to_string(event.val1) + " dmg (HP Left: " + std::to_string(event.val2) + ")";
            break;
            
        case EventType::UnitDied:
            msg_text = "[DEATH] " + event.message;
            break;
            
        case EventType::ScoreGained:
            msg_text = "[SCORE] " + event.message + " +" + std::to_string(event.val1) + " (Total: " + std::to_string(event.val2) + ")";
            break;
            
        case EventType::ScoreSpent:
            msg_text = "[SHOP] " + event.message + " cost " + std::to_string(event.val1) + " (Left: " + std::to_string(event.val2) + ")";
            break;
            
        case EventType::WeaponSwitched:
            msg_text = "[EQUIP] " + event.message;
            break;

        case EventType::SpellCast:
            msg_text = "[MAGIC] Cast spell: " + event.message;
            break;

        case EventType::SpellAcquired:
            msg_text = "[LOOT] Acquired spell: " + event.message;
            break;

        case EventType::Healed:
            msg_text = "[HEAL] " + event.message + " (HP: " + std::to_string(event.val1) + ")";
            break;

        case EventType::EnhancementAdded:
            msg_text = "[BUFF] " + event.message + " (Stacks: " + std::to_string(event.val1) + ")";
            break;
            
        default:
            msg_text = "[INFO] " + event.message;
            break;
    }
    write(msg_text);
}