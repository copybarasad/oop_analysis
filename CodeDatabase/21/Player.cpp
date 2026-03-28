#include "Player.hpp"

Player::Player(
    int playerAttack,
    int playerHealth,
    int playerLevel,
    long long prevLevelUpExperience,
    long long playerExperience,
    long long levelUpExperience,
    int playerIntelligence,
    int playerDexterity,
    int playerStrength,
    int playerHandSize
) {

    this->playerAttack = Attack{playerAttack};
    this->playerHealth = Health{playerHealth};
    this->playerHand = Hand{playerHandSize};
    this->playerStats = Stats{prevLevelUpExperience, playerExperience, levelUpExperience, playerLevel};
    this->playerAttributes = Attributes{playerIntelligence, playerDexterity, playerStrength};
}


Player::Player() {

}

Player::Player(PlayerSaveData data) {
    (void)data;
}

bool Player::checkDebaffState() {
    if (slowed) {
        slowed = false;
        return true;
    }
    return false;
}


void Player::setDebaffState() {
    slowed = true;
}


bool Player::alive() const {
    return playerHealth.getCurrentHealth();
}


int Player::getDamage() {
    int baseDamage = playerAttack.getAttack();
    int equippedWeaponIncrease = playerEquipment.getAttackBonus();
    int attributeBonus;
    if (playerEquipment.melle()) {
        attributeBonus = playerAttributes.getStrength();
    }
    else {
        attributeBonus = playerAttributes.getDexterity();
    }
    return (baseDamage + attributeBonus) * equippedWeaponIncrease;
}


std::pair<int, int> Player::getHealth() {
    return std::make_pair(playerHealth.getCurrentHealth(), playerHealth.getMaxHealth());
}


void Player::causeDamage(int damage) {
    playerHealth.reduseCurrentHealth(damage);
}


void Player::swapWeapon() {
    playerEquipment.swapWeapon();
}


int Player::getInt() {
    return playerAttributes.getIntelligence();
}


int Player::getDex() {
    return playerAttributes.getDexterity();
}


int Player::getStr() {
    return playerAttributes.getStrength();
}


int Player::getPowerOfSpell() {
    return playerHand.getPowerOfSpell();
}


void Player::setPowerOfSpell(int newPower) {
    playerHand.setPowerOfSpell(newPower);
}


void Player::setInt(int newInt) {
    playerAttributes.setIntelligence(newInt);
}


void Player::setDex(int newDex) {
    playerAttributes.setDexterity(newDex);
}


void Player::setStr(int newStr) {
    playerAttributes.setStrength(newStr);
    playerHealth.updateHealth(newStr);
}


bool Player::melle() {
    return playerEquipment.melle();
}


std::vector<long long> Player::getExperience() {
    return playerStats.getExperience();
}


int Player::getLevel() {
    return playerStats.getLevel();
}


bool Player::isLevelIncreased() {
    return playerStats.checkAndSwitchLevelIncreased();
}


void Player::addExperience(int exp) {
    playerStats.addExperience(exp);
}


void Player::addSpells(int indexInHand, int count) {
    playerHand.addSpells(indexInHand, count);
}


void Player::useItem(GameContext &ctx, int userIndex, int power) {
    playerHand.useItem(ctx, userIndex, power);
}


void Player::regenerateLife(float procrecoveryPercentage) {
    int currentMaxHealth = playerHealth.getMaxHealth();
    playerHealth.regenerate(currentMaxHealth * procrecoveryPercentage);
}


void Player::swapItemInHand(int itemIndexInHand) {
    playerHand.swapItem(itemIndexInHand);
}

std::vector<std::pair<bool, int>> Player::getHandItems() {
    return playerHand.getHandData();
}


std::pair<int, int> Player::getHandSize() {
    return {playerHand.getCurrentHandSize(), playerHand.getMaxHandSize()};
}


char Player::returnEntitySymbol() {
    return 'P';
}


PlayerSaveData Player::getPlayerSaveData() {
    PlayerSaveData data;
    data.playerAttack = this->playerAttack.getAttackSavedata();
    data.playerAttributes = this->playerAttributes.getAttributesSavedata();
    data.playerEquipment = this->playerEquipment.getEquipmentSavedata();
    data.playerHand = this->playerHand.getHandSavedata();
    data.playerHealth = this->playerHealth.getHealthSavedata();
    data.playerStats = this->playerStats.getStatsSavedata();
    data.slowed = (int)this->slowed;
    return data;
}


void Player::setPlayerSaveData(PlayerSaveData data) {
    this->playerAttack.setAttackSaveData(data.playerAttack);
    this->playerAttributes.setAttributesSavedata(data.playerAttributes);
    this->playerEquipment.setEquipmentSavedata(data.playerEquipment);
    this->playerHand.setHandSavedata(data.playerHand);
    this->playerHealth.setHealthSavedata(data.playerHealth);
    this->playerStats.setStatsSavedata(data.playerStats);
    this->slowed = data.slowed;
}


void Player::goToTheNextLevel() {
    regenerateLife(1.0f);
    playerHand.deleteHalfSpells();
}