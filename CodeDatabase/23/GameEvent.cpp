#include "GameEvent.h"
#include <iomanip>
#include <sstream>

GameEvent::GameEvent() { timestamp = std::time(nullptr); }

std::string GameEvent::getFormattedTime() const {
  std::tm *timeInfo = std::localtime(&timestamp);
  std::ostringstream oss;
  oss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}


DamageEvent::DamageEvent(const std::string &attacker, const std::string &target,
                         int damage)
    : attacker(attacker), target(target), damage(damage) {}

std::string DamageEvent::getDescription() const {
  std::ostringstream oss;
  oss << attacker << " dealt " << damage << " damage to " << target;
  return oss.str();
}


MoveEvent::MoveEvent(int fromX, int fromY, int toX, int toY)
    : fromX(fromX), fromY(fromY), toX(toX), toY(toY) {}

std::string MoveEvent::getDescription() const {
  std::ostringstream oss;
  oss << "Player moved from (" << fromX << ", " << fromY << ") to (" << toX
      << ", " << toY << ")";
  return oss.str();
}


SpellEvent::SpellEvent(const std::string &spellName,
                       const std::string &eventType, int targetX, int targetY)
    : spellName(spellName), eventType(eventType), targetX(targetX),
      targetY(targetY) {}

std::string SpellEvent::getDescription() const {
  std::ostringstream oss;
  if (eventType == "acquired") {
    oss << "Acquired spell: " << spellName;
  } else if (eventType == "cast") {
    oss << "Cast spell: " << spellName;
    if (targetX >= 0 && targetY >= 0) {
      oss << " at (" << targetX << ", " << targetY << ")";
    }
  }
  return oss.str();
}


EnemyDeathEvent::EnemyDeathEvent(const std::string &enemyType, int x, int y)
    : enemyType(enemyType), x(x), y(y) {}

std::string EnemyDeathEvent::getDescription() const {
  std::ostringstream oss;
  oss << enemyType << " defeated at (" << x << ", " << y << ")";
  return oss.str();
}


LevelCompleteEvent::LevelCompleteEvent(int level, int score)
    : level(level), score(score) {}

std::string LevelCompleteEvent::getDescription() const {
  std::ostringstream oss;
  oss << "Level " << level << " completed! Score: " << score;
  return oss.str();
}


PlayerDeathEvent::PlayerDeathEvent(int level, int score)
    : level(level), score(score) {}

std::string PlayerDeathEvent::getDescription() const {
  std::ostringstream oss;
  oss << "Player died at level " << level << " with score: " << score;
  return oss.str();
}
