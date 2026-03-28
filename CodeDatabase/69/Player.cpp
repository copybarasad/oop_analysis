#include "Player.h"
#include <iostream>

Player::Player(Position pos, size_t handCapacity, int hp, int coins)
    : pos_(pos)
    , hand_(handCapacity)
    , accumulatedBuff_()
    , coins_(coins)
    , hp_(hp)
    , maxHp_(50)
{}

Position Player::GetPosition() const { 
    return pos_; 
}

void Player::SetPosition(Position p) { 
    pos_ = p; 
}

Hand& Player::GetHand() { 
    return hand_; 
}

const Hand& Player::GetHand() const { 
    return hand_; 
}

// бафф к накопленному
void Player::AddBuff(BuffState const& bs) { 
    accumulatedBuff_ += bs;
    std::cout << "✨ Buff accumulated! (Damage +" << bs.damageRangeBonus
              << ", Area +" << bs.areaSizeBonus
              << ", Trap +" << bs.trapDamageBonus
              << ", Summon +" << bs.summonCountBonus << ")\n";
}


BuffState const& Player::GetAccumulatedBuff() const { 
    return accumulatedBuff_; 
}

void Player::ClearBuffs() { 
    accumulatedBuff_ = BuffState{}; 
}

int Player::GetCoins() const { 
    return coins_; 
}

void Player::AddCoins(int v) { 
    coins_ += v; 
}

bool Player::Move(Position delta, Game const& game) 
{
    Position np{pos_.x + delta.x, pos_.y + delta.y};

    if (!game.IsInBounds(np))
        return false;

    if (game.IsOccupied(np))
        return false;

    pos_ = np;
    return true;
}

// TODO: remove
void Player::ApplyAccumulatedBuffsToSpell(Spell& spell) 
{
    if (accumulatedBuff_.damageRangeBonus ||
        accumulatedBuff_.areaSizeBonus ||
        accumulatedBuff_.trapDamageBonus ||
        accumulatedBuff_.summonCountBonus) 
    {
        spell.ApplyBuffs(accumulatedBuff_);
        std::cout << "Applied accumulated buffs to " << spell.GetName() << "!\n";
        ClearBuffs();
    }
}

bool Player::SpendCoins(int v) 
{
    if (coins_ < v)
        return false;

    coins_ -= v;
    return true;
}

void Player::CollectCoinsFromGame(Game& game) 
{
    int got = game.FetchAndClearCoinPool();

    if (got > 0) {
        AddCoins(got);
        std::cout << "💰 Collected " << got << " coins from recent kills. Total coins: " << GetCoins() << "\n";
    }
}

int Player::GetHp() const { 
    return hp_; 
}

void Player::TakeDamage(int dmg) 
{
    hp_ -= dmg;
    if (hp_ < 0)
        hp_ = 0;
}

bool Player::IsAlive() const { 
    return hp_ > 0; 
}
