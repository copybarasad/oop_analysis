#include "player.hpp"
#include "enemy.hpp"
#include <vector>

#define SCORE 10

Player::Player(int hp, int damage)
    : hp(hp), damage(damage), score(0), x(0), y(0), melee_damage(damage),
      ranged_damage(damage - 5), is_ranged(false) {}

void Player::switch_style() {
  is_ranged = !is_ranged;
  damage = is_ranged ? ranged_damage : melee_damage;
}

void Player::take_damage(int damag) {
  hp -= damag;
  if (hp <= 0) {
    hp = 0;
  }
}

bool Player::is_alive() const { return hp > 0; }

void Player::add_score(int points) { score += points; }

int Player::get_damage() const { return damage; }

int Player::get_hp() const { return hp; }

int Player::get_x() const { return x; }

int Player::get_y() const { return y; }

void Player::move(int dx, int dy, const std::vector<std::vector<Cell>> &field, std::vector<Enemy>& enemies) {
  int new_x = x + dx;
  int new_y = y + dy;
  bool enemyFound = false;
  for (auto& enemy : enemies) {
    if (enemy.is_alive() && enemy.get_x() == new_x && enemy.get_y() == new_y) {
      //проверка
      if(!is_ranged){
        enemy.take_damage(damage);
        if (!enemy.is_alive()) {
          add_score(SCORE);
        }
      }
      enemyFound = true;//нашли врага
      break;
    }
  }
  if (enemyFound) {
    return;
  }
  if (new_x >= 0 && new_x < field.size() && new_y >= 0 &&
      new_y < field[0].size() && field[new_x][new_y].get_isPass()) {
    x = new_x;
    y = new_y;
  }
}

bool Player::getIsRangedCombat() const { return is_ranged; }

int Player::get_score() const { return score; }

void Player::rangedAttack(int dx, int dy, std::vector<Enemy>& enemies) {
  if (!is_ranged){
    return;
  }
  int attack_x = x;
  int attack_y = y;
  for (int i = 1; i <= 3; i++) {
    attack_x += dx;
    attack_y += dy;
    for (auto& enemy : enemies) {
      if (enemy.is_alive() && enemy.get_x() == attack_x && enemy.get_y() == attack_y) {
        enemy.take_damage(damage);
        if (!enemy.is_alive()) {
          add_score(SCORE);
        }
        return;
      }
    }
  }
}

bool Player::spendScore(int amount) {
  if (score >= amount) {
      score -= amount;
      return true;
  }
  return false;
}

