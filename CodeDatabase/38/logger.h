#pragma once
#include <iostream>
#include <sstream>
enum class Events {
  PLAYER_MOVED,
  PLAYER_HEALTH_ADDED,
  PLAYER_DAMAGED,
  ENEMY_DAMAGED,
  ENEMY_SPAWNED,
  ENEMY_KILLED,
  TOWER_DAMAGED,
  TOWER_DESTROYED,
  TOWER_SPAWNED,
  TOWER_ATTACKED,
  CARD_ADDED,
  SPELL_CAST,
  AREA_SPELL,
  TRAP_PLACED,
  TRAP_TRIGGERED,
  GAME_STARTED,
  GAME_ENDED,
};
typedef struct {
  int x;
  int y;
  int dmg;
  std::string &spell_name;

} Metadata;
class Logger {
private:
  std::stringstream ss;

public:
  Logger() {}
  std::string eventTypeToString(Events type) const;
  void logEvent(Events event, Metadata *meta);
  void log(std::string event, std::string message);
  void clearMetadata(Metadata &meta);
};
