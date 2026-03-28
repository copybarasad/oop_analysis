#include "logger.h"
#include <algorithm>
#include <iostream>
#include <string>
void Logger::log(std::string event, std::string message) {
  std::cout << "\033[36m" << "[" << event << "] \033[0m" << message
            << std::endl;
}
void clearMetadata(Metadata &meta) {}
void Logger::logEvent(Events event, Metadata *meta = NULL) {

  switch (event) {
  case Events::PLAYER_MOVED: {
    std::string l = "PLAYER MOVED TO ";
    l += std::to_string(meta->x);
    l += ",";
    l += std::to_string(meta->y);
    log("PLAYER", l);
  } break;
  case Events::PLAYER_DAMAGED:
    log("\033[31mPLAYER\033[0m", "PLAYER DAMAGED");
    break;
  case Events::PLAYER_HEALTH_ADDED:

    log("PLAYER", "PLAYER HEALED");
    break;
  case Events::ENEMY_SPAWNED:
    log("Events", "ENEMY SPAWNED");
    break;
  case Events::ENEMY_DAMAGED:
    log("ENEMY", "ENEMY DAMAGED");
    break;
  case Events::ENEMY_KILLED:
    log("ENEMY", "ENEMY KILLED");
    break;
  case Events::TOWER_DAMAGED:
    log("TOWER", "TOWER DAMAGED");
    break;
  case Events::TOWER_DESTROYED:
    log("TOWER", "TOWER DESTROYED");
    break;
  case Events::SPELL_CAST:
    log("SPELL", "SPELL CAST");
    break;
  case Events::AREA_SPELL:
    log("SPELL", "AREA SPELL");
    break;
  case Events::TOWER_SPAWNED:
    log("TOWER", "TOWER SPAWNED");
    break;
  case Events::CARD_ADDED:
    log("HAND", "CARD ADDED");
    break;
  case Events::TOWER_ATTACKED:
    log("TOWER", "TOWER ATTACKED");
    break;

  case Events::TRAP_PLACED:
    log("TRAP", "TRAP PLACED");
    break;
  case Events::TRAP_TRIGGERED:
    log("TRAP", "TRAP TRIGGERED");
    break;
  case Events::GAME_STARTED:
    log("GAME", "GAME STARTED");
    break;
  case Events::GAME_ENDED:
    log("\033[31mGAME\033[0m", "GAME OVER");
    break;
  default:
    break;
  }
}

std::string Logger::eventTypeToString(Events type) const {
  switch (type) {
  case Events::PLAYER_MOVED:
    return "PLAYER MOVED";
  case Events::PLAYER_DAMAGED:
    return "PLAYER DAMAGED";
  case Events::ENEMY_DAMAGED:
    return "ENEMY DAMAGED";
  case Events::ENEMY_KILLED:
    return "ENEMY KILLED";
  case Events::TOWER_DAMAGED:
    return "TOWER DAMAGED";
  case Events::TOWER_DESTROYED:
    return "TOWER DESTROYED";
  case Events::SPELL_CAST:
    return "SPELL CAST";
  case Events::TRAP_PLACED:
    return "TRAP PLACED";
  case Events::TRAP_TRIGGERED:
    return "TRAP TRIGGERED";
  case Events::GAME_STARTED:
    return "GAME STARTED";
  case Events::GAME_ENDED:
    return "GAME ENDED";
  default:
    return "UNKNOWN";
  }
}
void Logger::clearMetadata(Metadata &meta) {
  meta.x = 0;
  meta.y = 0;
  meta.dmg = 0;
  meta.spell_name = "";
}
