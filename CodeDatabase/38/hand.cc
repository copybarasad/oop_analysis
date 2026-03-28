#include "hand.h"
#include "enemies.h"

#include <raylib.h>
unsigned int Hand::get_max() const { return max_amount; }
unsigned int Hand::get_amount() const { return hand.size(); }
const std::map<int, int> &Hand::showHand() const { return hand; }
bool Hand::useSpell(Spell::SpellType type, std::string &persistent_str,
                    DirectDamage &dird, AreaDamage &ad, Player *player,
                    Enemies &enemies, Field *field) {

  for (auto &p : hand) {
    if (p.first == type) {

      if (p.second > 0) {
        p.second--;
        switch (type) {
        case Spell::DIRECT:
          if (!dird.doDamage(*player, enemies.enemies, *field)) {
            persistent_str = "Cannot use direct attack!\nYOUR TURN!";
            addCard(Spell::DIRECT);
            return false;
          }
          break;

        case Spell::AREA:
          if (!ad.doDamage(*player, enemies.enemies, *field)) {
            persistent_str = "Cannot use area attack!\nYOUR TURN!";
            addCard(Spell::AREA);
            return false;
          }
          break;
        default:
          break;
        }
        return true;
      }
    }
  }
  persistent_str = "Not enough Direct Damage cards! \nYOUR TURN";

  return false;
}
void Hand::addCard(Spell::SpellType type) {
  if (hand.size() == max_amount) {
    std::cout << "MAX HAND SIZE" << std::endl;
    return;
  }
  bool exists = false;
  switch (type) {
  case Spell::DIRECT:
    std::cout << "DIRECT" << std::endl;
    for (auto &p : hand) {
      if (p.first == Spell::DIRECT) {
        exists = true;
        p.second++;
        std::cout << p.second << std::endl;
      }
    }
    if (!exists) {
      hand.insert({Spell::DIRECT, 1});
      // delete d;
    }

    break;

  case Spell::SpellType::AREA:
    for (auto &p : hand) {
      if (p.first == Spell::AREA) {
        exists = true;
        p.second++;
        std::cout << p.second << std::endl;
      }
    }
    hand.insert({Spell::AREA, 1});
    // delete a;

    break;
  case Spell::TRAP:
    exists = false;

    for (auto &p : hand) {
      if (p.first == Spell::TRAP) {
        exists = true;
        p.second++;
      }
    }
    if (!exists) {
      hand.insert({Spell::TRAP, 1});
    }
    break;
  default:
    break;
  }
}
Hand::Hand(unsigned int max) : max_amount(max) {
  if (max == 0)
    std::invalid_argument("Max hand size has to be more than 0");
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1);
  Spell::SpellType type = (Spell::SpellType)(dist(mt));
  addCard(type);
}
unsigned int Hand::getCardAmount(Spell::SpellType type) {
  for (auto &deck : hand) {
    if (deck.first == type)
      return deck.second;
  }

  return 0;
}
void Hand::DrawHand() {
  int round = 0;
  for (const auto &p : hand) {
    switch (p.first) {
    case Spell::SpellType::DIRECT:
      DrawText(TextFormat("\tDirect: %d", p.second), 820, 200 + 50 * round, 48,
               GREEN);
      break;

    case Spell::SpellType::AREA:
      DrawText(TextFormat("\tArea: %d", p.second), 820, 200 + 50 * round, 48,
               RED);
      break;

    case Spell::SpellType::TRAP:

      DrawText(TextFormat("\tTrap: %d", p.second), 820, 200 + 50 * round, 48,
               YELLOW);
      break;

    default:

      DrawText(TextFormat("\tUnknown"), 820, 200 + 50, 48, RED);
      break;
    }
    round++;
  }
}
