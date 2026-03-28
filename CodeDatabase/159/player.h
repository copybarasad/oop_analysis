#ifndef PLAYER_H
#define PLAYER_H

#include "spell.h"
#include "spellHand.h"
#include <memory>

class Player {
public:
  Player(int startX, int startY, int initialHealth, int meleeDamage, int rangedDamage, int initialScore, int initialAmmo, size_t maxSize);

  enum Mode {Melee, Ranged};

  int getX() const;
  int getY() const;
  int getHealth() const;
  int getDamage() const;
  int getScore() const;
  int getAmmo() const;
  Mode getMode() const;
  int getSpellDamage() const;
  int getEnhancementLevel() const;
  const SpellHand& getSpellHand() const;
  SpellHand& getSpellHand();

  void setHealth(int h);
  void setScore(int s);
  void setPosition(int x, int y);
  void setAmmo(int a);
  void setMode(Mode m);
  void setEnhancementLevel(int level);

  void move(int newX, int newY);
  void takeDamage(int damage);
  void addScore(int points);
  void switchMode();
  void reloadAmmo(int count);
  void useAmmo(int count);
  void setSlowed(bool slowed);
  void castSpell(int spellIndex, int tx, int ty, Game& game);
  void increaseEnhancement();
  bool buySpell();
  bool isAlive() const;
  bool isSlowed() const;

private:
  int x, y;
  int health;
  int meleeDamage;
  int rangedDamage;
  int spellDamage;
  Mode currentMode;
  int score;
  int ammo;
  bool slowed;
  SpellHand hand;
  int enhancementLevel;
};

#endif