#include "player.h"
#include "directDamageSpell.h"
#include "areaDamageSpell.h"
#include "trapSpell.h"
#include "summonAllySpell.h"
#include "enhancementSpell.h"
#include <iostream>
#include <algorithm>

Player::Player(int startX, int startY, int initialHealth, int meleeDmg, int rangedDmg, int initialScore, int initialAmmo, size_t maxSize):
  x(startX), y(startY), health(initialHealth), meleeDamage(meleeDmg), 
  rangedDamage(rangedDmg), spellDamage(30), currentMode(Melee), score(initialScore), 
  ammo(initialAmmo), slowed(false), hand(maxSize), enhancementLevel(0) {
    hand.addRandomSpell();
}

int Player::getX() const {return x;}
int Player::getY() const {return y;}
int Player::getHealth() const {return health;}
Player::Mode Player::getMode() const {return currentMode;}
int Player::getDamage() const {
  return (currentMode == Melee) ? meleeDamage : rangedDamage;
}
int Player::getAmmo() const {return ammo;}
int Player::getScore() const {return score;}
int Player::getSpellDamage() const {return spellDamage;}
int Player::getEnhancementLevel() const {return enhancementLevel;}
const SpellHand& Player::getSpellHand() const {return hand;}
SpellHand& Player::getSpellHand() {return hand;}

void Player::setHealth(int h) { health = h; }
void Player::setScore(int s) { score = s; }
void Player::setPosition(int x, int y) { this->x = x, this->y = y; }
void Player::setAmmo(int a) { ammo = a; }
void Player::setMode(Mode m) { currentMode = m; }
void Player::setEnhancementLevel(int level) { enhancementLevel = level; }

void Player::move(int newX, int newY) {
  x = newX;
  y = newY;
}

void Player::takeDamage(int initialDamage) {
  health -= initialDamage;
  if (health<0) health = 0;
}

void Player::addScore(int points) {
  score += points;
}

void Player::switchMode() {
  currentMode = (currentMode == Melee) ? Ranged : Melee;
}

void Player::reloadAmmo(int count) {
  ammo += count;
}

void Player::useAmmo(int count) {
  ammo -= count;
  ammo = std::max(ammo, 0);
}

void Player::setSlowed(bool slowed) {
  this->slowed = slowed;
}

bool Player::isAlive() const {
  return health>0;
}

bool Player::isSlowed() const {
  return slowed;
}

void Player::castSpell(int spellIndex, int tx, int ty, Game& game) {
  const Spell& spell = hand.selectSpell(spellIndex);
  spell.apply(game, tx, ty, x, y);
  if (spell.resetsEnhancement()) {
    enhancementLevel = 0;
  }
  hand.removeSpell(spellIndex);
}

void Player::increaseEnhancement() {
  enhancementLevel++;
}

bool Player::buySpell() {
  const int cost = 20;
  if (score<cost) {
    std::cout << "Not enough score for purchase (need " << cost << ").\n";
    return false;
  }
  if (hand.getSize()>=hand.getMaxSize()) {
    std::cout << "Hand is full, cannot buy.";
    return false;
  }

  auto descriptions = hand.getAvailableSpellDescriptions();
  if (descriptions.empty()) {
    std::cout << "No spells available for purchase.\n";
    return false;
  }

  std::cout << "Available spells (cost: " << cost << " score):\n";
  for (size_t i=0; i<descriptions.size(); ++i) {
    std::cout << (i+1) << ". " << descriptions[i] << "\n";
  }
  std::cout << "Choose number (or 0 to cancel): ";

  int choice;
  std::cin >> choice;
  if (choice==0 || choice<1 || choice>static_cast<int>(descriptions.size())) {
    std::cout << "Purchase canceled.\n";
    return false;
  }

  SpellType selectedType = static_cast<SpellType>(choice - 1);
  auto selectedSpell = hand.createSpell(selectedType);
  if (selectedSpell && hand.addSpell(std::move(selectedSpell))) {
    score -= cost;
    std::cout << "Spell purchased and added to hand.\n";
    return true;
  }
  return false;
}