#include <utility>
#include <memory>
#include "PlayerManager.h"

PlayerManager::PlayerManager()
    : player_pos({0, 0}), player() {}

PlayerManager::PlayerManager(dataPlayer data) : player_pos(data.pos), player(data){}

PlayerManager::~PlayerManager() = default;

void PlayerManager::SetPlayerPosition(std::pair<int, int> pos) {
    player_pos = pos;
}
int PlayerManager::GetPlayerHealth() const {
    return player.GetHealth();
}
int PlayerManager::GetPlayerDamage() const {
    return player.GetDamage();
}
void PlayerManager::TakePlayerDamage(int amount) {
    player.TakeDamage(amount);
}
bool PlayerManager::IsPlayerAlive() const {
    return player.IsAlive();
}
std::pair<int, int> PlayerManager::GetPlayerPosition() const {
    return player_pos;
}

int PlayerManager::GetPlayerAttackRange() {
    return (player.GetAttackMode() == AttackMode::Ranged) ? 2 : 1;
}

void PlayerManager::SwitchPlayerAttackMode() {
    player.SwitchAttackMode();
    player.SetStunDuration(1);
}

bool PlayerManager::IsPlayerStunned() const {
    return player.IsStunned();
}

void PlayerManager::DecreasePlayerStunDuration() {
    player.DecreaseStunDuration();
}

void PlayerManager::SetPlayerStunDuration(int d) {
    player.SetStunDuration(d);
}

dataPlayer PlayerManager::Save(){
    dataPlayer data;
    data.pos=player_pos;
    data.mode=player.GetAttackMode();
    data.score=player.GetScore();
    data.damage=player.GetDamage();
    data.health=player.GetHealth();
    data.stunDuration=player.GetStunDuration();
    return data;
}