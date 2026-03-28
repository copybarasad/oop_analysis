#include "EventLogger.h"
#include "Logger.h" 
#include <iostream>

EventLogger::EventLogger(LogOutput type, const std::string& filename) 
    : outputType(type) 
{
    if (type == LogOutput::FILE || type == LogOutput::BOTH) {
        fileStream.open(filename, std::ios::app);
        if (!fileStream.is_open()) {
            std::cerr << "Failed to open log file. Logs will only appear in console.\n";
            outputType = LogOutput::CONSOLE;
        }
    }
}

EventLogger::~EventLogger() {
    if (fileStream.is_open()) fileStream.close();
}

std::string EventLogger::resolveMessage(LogMsg msg) {
    switch (msg) {
        case LogMsg::FIELD_LOAD_ERROR: return "Error: Failed to load field data";
        case LogMsg::FIELD_SAVE_ERROR: return "Error: Failed to save field data.";
        case LogMsg::FIELD_INVALID_SIZE: return "Warning: Invalid field size detected";

        case LogMsg::GAME_STARTED: return "Game Started!";
        case LogMsg::GAME_RESTARTED: return "Game Restarted!";
        case LogMsg::GAME_OVER: return "GAME OVER!";
        case LogMsg::EXITING: return "Exiting game...";
        
        case LogMsg::HIT_WALL: return "You hit a wall!";
        case LogMsg::CELL_OCCUPIED: return "Cell is occupied!";
        case LogMsg::STEPPED_TRAP: return "You stepped on a slowing trap!";
        case LogMsg::ATTACK_MELEE: return "You attacked a target in melee!";
        case LogMsg::ATTACK_RANGED: return "You fired a projectile!";
        case LogMsg::MOVED_SLOWED: return "You are slowed by a trap and must skip this turn!";
        
        case LogMsg::SPELL_CAST_CANCEL: return "Spell cast cancelled.";
        case LogMsg::SPELL_NO_SPELLS: return "No spells to cast!";
        case LogMsg::SPELL_INVALID_INDEX: return "Invalid spell index.";
        case LogMsg::SPELL_HAND_FULL: return "Hand is full!";
        case LogMsg::SPELL_FIREBALL_HIT: return "Fireball hit a target!";
        case LogMsg::SPELL_FAIL: return "Invalid target";
        case LogMsg::SPELL_FIREBALL_WALL: return "Fireball hit a wall.";
        case LogMsg::SPELL_ZAP_HIT: return "Zap strikes the area!";
        case LogMsg::SPELL_TRAP_PLACED: return "A trap has been placed.";
        case LogMsg::SPELL_TRAP_FAIL: return "Cannot place a trap there. Occupied or out of bounds.";
        case LogMsg::SPELL_SUMMON_FAIL: return "No space";
        case LogMsg::SPELL_BUFF: return "Your next spell will be enhanced.";

        case LogMsg::ENEMY_STEPPED_TRAP: return "Enemy stepped on a trap!";
        case LogMsg::TOWER_HIT_PLAYER: return "An enemy tower shot a fireball at you!";
        
        case LogMsg::SAVE_SUCCESS: return "Game quick-saved!";
        case LogMsg::LOAD_SUCCESS: return "Game quick-loaded!";
        
        case LogMsg::UPGRADE_HEALTH: return "Upgraded Max Health!";
        case LogMsg::UPGRADE_DAMAGE: return "Upgraded Damage!";
        
        default: return "Unknown Event";
    }
}


void EventLogger::log(LogMsg msg) {
    writeString(resolveMessage(msg));
}

void EventLogger::log(LogMsg msg, int value) {
    std::string text = resolveMessage(msg); 
    
    switch (msg) {
        case LogMsg::LEVEL_START: text = "--- Started Level " + std::to_string(value) + " ---"; break;
        case LogMsg::LEVEL_COMPLETE: text = "Level " + std::to_string(value) + " Complete!"; break;
        case LogMsg::SPELL_NOT_ENOUGH_SCORE: text = "Not enough score! Need " + std::to_string(value); break;
        case LogMsg::SPELL_SUMMON: text = "You summoned " + std::to_string(value) + " ally/allies!"; break;
        case LogMsg::ENEMY_HIT_PLAYER: text = "Enemy hit Player for " + std::to_string(value) + " damage!"; break;
        case LogMsg::ENTITY_DESTROYED: text = "Entity destroyed! +" + std::to_string(value) + " pts"; break;
        case LogMsg::FATIGUE_LOST_SPELLS: text = "Lost " + std::to_string(value) + " spells due to fatigue."; break;
        default: text += " " + std::to_string(value); break;
    }
    writeString(text);
}

void EventLogger::log(LogMsg msg, const std::string& textVal) {
    std::string text;
    switch (msg) {
        case LogMsg::SPELL_BOUGHT: text = "Bought: " + textVal; break;
        case LogMsg::SWITCH_MODE: text = "Switched to " + textVal + " mode."; break;
        case LogMsg::SAVE_FAIL: text = "Save failed: " + textVal; break;
        case LogMsg::LOAD_FAIL: text = "Load failed: " + textVal; break;
        case LogMsg::GAME_OVER: text = "GAME OVER! Final Score: " + textVal; break;
        default: text = resolveMessage(msg) + " " + textVal; break;
    }
    writeString(text);
}


void EventLogger::writeString(const std::string& message) {
    if (outputType == LogOutput::CONSOLE || outputType == LogOutput::BOTH) {
        Logger::logMessage(message); 
    }
    if (outputType == LogOutput::FILE || outputType == LogOutput::BOTH) {
        if (fileStream.is_open()) {
            fileStream << "[EVENT]: " << message << std::endl;
        }
    }
}