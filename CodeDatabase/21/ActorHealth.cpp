#include "ActorHealth.hpp"

Health::Health() : Health(DEFAULT_HEALTH){}

Health::Health(int health) {
    currentHealth = health;
    maxHealth = health;
}


Health::~Health() {
}


void Health::reduseCurrentHealth(int damage) {
    currentHealth = currentHealth - damage;
}


void Health::regenerate(int regenerateAmount) {
    if (currentHealth + regenerateAmount < maxHealth) {
        currentHealth = currentHealth + regenerateAmount;
    }
    else {
    currentHealth = maxHealth;
    }
}


void Health::restoreHealth() {
    currentHealth = maxHealth;
}


void Health::setMaxHealth(int newMaxHealth) {
    maxHealth = newMaxHealth;
}


void Health::updateHealth(int str) {
    int prevMaxHealth = maxHealth;
    int newMaxHealth = prevMaxHealth + str * 10;
    setMaxHealth(newMaxHealth);
    restoreHealth();
}


int Health::getCurrentHealth() const {
    return currentHealth < 0 ? 0 : currentHealth;
}

int Health::getMaxHealth() const {
    return maxHealth;
}


HealthSaveData Health::getHealthSavedata() {
    HealthSaveData data;
    data.currentHealth = this->currentHealth;
    data.maxHealth = this->maxHealth;
    return data;
}


void Health::setHealthSavedata(HealthSaveData data) {
    this->currentHealth = data.currentHealth;
    this->maxHealth = data.maxHealth;
}