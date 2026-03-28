

#include "AoESpell.h"
#include "BuffSpell.h"
#include "SummonSpell.h"
#include "TrapSpell.h"
Player::Player(int hp, int dmg, Position pos)
    : Entity(pos, hp),
      damage(dmg),
      score(0),
      slowed(false),
      mode(CombatMode::Melee),
      hand(),
      pendingBuffs(0) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 5);

  std::unique_ptr<ISpell> spell;
  int choice = dis(gen);
  switch (choice) {
    case 1:
      spell = std::make_unique<DamageSpell>(8, 3);
      break;
    case 2:
      spell = std::make_unique<AoESpell>(10, 2);
      break;
    case 3:
      spell = std::make_unique<TrapSpell>(5);
      break;
    case 4:
      spell = std::make_unique<SummonSpell>(1);
      break;
    case 5:
      spell = std::make_unique<BuffSpell>();
      break;
  }

  if (spell) {
    hand.AddSpell(std::move(spell));
  }
}

void Player::SwitchCombatMode() {
  if (mode == CombatMode::Melee) {
    mode = CombatMode::Ranged;
    SetSlowed();
    damage = 5;
  } else {
    mode = CombatMode::Melee;
    SetSlowed();
    damage = 10;
  }
}

void Player::ResetStatus() { slowed = false; }
int Player::GetDamage() const { return damage; }
Player::CombatMode Player::GetMode() const { return mode; }
void Player::SetSlowed() { slowed = true; }
void Player::AddScore(int points) { score += points; }
int Player::GetScore() const { return score; }
bool Player::IsSlowed() const { return slowed; }
void Player::RestoreFullHealth() { Entity::SetHealth(maxHealth); }

Hand& Player::GetHand() { return hand; }

void Player::SetHand(Hand&& h) { hand = std::move(h); }

void Player::AddPendingBuff() { ++pendingBuffs; }

int Player::ConsumePendingBuffs() {
  int b = pendingBuffs;
  pendingBuffs = 0;
  return b;
}

void Player::PrintHand() {
  std::cout << "\n--- Player's Hand ---\n";
  auto& spells = hand.GetSpells();
  for (size_t i = 0; i < spells.size(); ++i) {
    std::cout << "[" << i << "] " << spells[i]->GetName() << "\n";
  }
  std::cout << "----------------------\n";
}

PlayerSaveData Player::Save() const {
  PlayerSaveData d;
  d.hp = GetHealth();
  d.max_hp = maxHealth;
  d.damage = damage;
  d.score = score;
  d.mode = (mode == CombatMode::Melee ? SavedCombatMode::Melee
                                      : SavedCombatMode::Ranged);
  d.pendingBuffs = pendingBuffs;
  return d;
}

void Player::LoadFromSave(const PlayerSaveData& data) {
  Entity::SetHealth(data.hp);
  maxHealth = data.max_hp;
  damage = data.damage;
  score = data.score;
  mode = (data.mode == SavedCombatMode::Melee ? CombatMode::Melee
                                              : CombatMode::Ranged);
  pendingBuffs = data.pendingBuffs;
}
