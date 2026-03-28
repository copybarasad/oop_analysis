#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <ctime>
#include <string>


class GameEvent {
protected:
  std::time_t timestamp;

public:
  GameEvent();
  virtual ~GameEvent() = default;
  virtual std::string getDescription() const = 0;
  virtual std::string getEventType() const = 0;
  std::time_t getTimestamp() const { return timestamp; }
  std::string getFormattedTime() const;
};


class DamageEvent : public GameEvent {
private:
  std::string attacker;
  std::string target;
  int damage;

public:
  DamageEvent(const std::string &attacker, const std::string &target,
              int damage);
  std::string getDescription() const override;
  std::string getEventType() const override { return "DAMAGE"; }
};


class MoveEvent : public GameEvent {
private:
  int fromX, fromY;
  int toX, toY;

public:
  MoveEvent(int fromX, int fromY, int toX, int toY);
  std::string getDescription() const override;
  std::string getEventType() const override { return "MOVE"; }
};


class SpellEvent : public GameEvent {
private:
  std::string spellName;
  std::string eventType; 
  int targetX, targetY;

public:
  SpellEvent(const std::string &spellName, const std::string &eventType,
             int targetX = -1, int targetY = -1);
  std::string getDescription() const override;
  std::string getEventType() const override { return "SPELL"; }
};


class EnemyDeathEvent : public GameEvent {
private:
  std::string enemyType;
  int x, y;

public:
  EnemyDeathEvent(const std::string &enemyType, int x, int y);
  std::string getDescription() const override;
  std::string getEventType() const override { return "ENEMY_DEATH"; }
};


class LevelCompleteEvent : public GameEvent {
private:
  int level;
  int score;

public:
  LevelCompleteEvent(int level, int score);
  std::string getDescription() const override;
  std::string getEventType() const override { return "LEVEL_COMPLETE"; }
};


class PlayerDeathEvent : public GameEvent {
private:
  int level;
  int score;

public:
  PlayerDeathEvent(int level, int score);
  std::string getDescription() const override;
  std::string getEventType() const override { return "PLAYER_DEATH"; }
};

#endif 
