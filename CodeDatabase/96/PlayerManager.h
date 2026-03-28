#pragma once
#include <utility>
#include <memory>
#include "Player.h"
#include "struct/dataPlayer.h"

class PlayerManager {
private:
    std::pair<int, int> player_pos;
    Player player;
public:
    PlayerManager();
    PlayerManager(dataPlayer data);
    ~PlayerManager();
    void SetPlayerPosition(std::pair<int, int> pos);
    int GetPlayerHealth() const;
    int GetPlayerDamage() const;
    void TakePlayerDamage(int amount);
    bool IsPlayerAlive() const;
    std::pair<int, int> GetPlayerPosition() const;
    void SwitchPlayerAttackMode();
    void AddPlayerScore(int s) { player.AddScore(s); }
    int GetPlayerAttackRange() ;
    bool IsPlayerStunned() const;
    void DecreasePlayerStunDuration();
    void SetPlayerStunDuration(int d);
    int GetPlayerScore() const { return player.GetScore(); }
    int GetMurders() {return player.GetMurders();}
    dataPlayer Save();

};