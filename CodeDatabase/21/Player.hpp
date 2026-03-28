#pragma once
#include "Entity.hpp"
#include "../actor_utility/ActorAttributes.hpp"
#include "../actor_utility/ActorAttack.hpp"
#include "../actor_utility/ActorEquipment.hpp"
#include "../actor_utility/ActorHand.hpp"
#include "../actor_utility/ActorHealth.hpp"
#include "../actor_utility/ActorInventory.hpp"
#include "../actor_utility/ActorStats.hpp"
#include "../items/DirectDamageSpell.hpp"
#include "../items/Item.hpp"

#include "../core/SaveData.hpp"

#include <utility>
#include <memory>

class Player : public Entity {
private:
    Attributes playerAttributes;
    Attack playerAttack;
    Equipment playerEquipment;
    Hand playerHand;
    Stats playerStats;
    Health playerHealth;
    bool slowed = false;
public:
    entityType getType() const override {
        return entityType::PLAYER;
    }
    Player();
    Player(PlayerSaveData data);
    Player(
        int playerAttack, int playerHealth, int playerLevel,
        long long prevLevelUpExperience, long long playerExperience,
        long long levelUpExperience, int intelligence, int dexterity, int strength,
        int playerHandSize
    );
    bool checkDebaffState() override;
    void setDebaffState() override;
    bool alive() const override;
    bool melle() override;
    int getDamage() override;
    std::pair<int, int> getHealth() override;
    void causeDamage(int damage) override;
    void swapWeapon() override;
    int getInt() override;
    int getDex() override;
    int getStr() override;
    int getPowerOfSpell() override;
    void setPowerOfSpell(int newPower) override;
    void setInt(int newInt) override;
    void setDex(int newDex) override;
    void setStr(int newStr) override;
    std::vector<long long> getExperience() override;
    int getLevel() override;
    bool isLevelIncreased() override;
    void addExperience(int exp) override;
    void addSpells(int indexInHand, int count) override;
    void useItem(GameContext &ctx, int userIndex, int power) override;
    void regenerateLife(float procrecoveryPercentage) override;
    void swapItemInHand(int itemIndexInHand) override;
    std::vector<std::pair<bool, int>> getHandItems();
    std::pair<int, int> getHandSize();
    char returnEntitySymbol() override;
    PlayerSaveData getPlayerSaveData();
    void setPlayerSaveData(PlayerSaveData data);
    void goToTheNextLevel() override;
};