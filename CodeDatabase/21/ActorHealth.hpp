#pragma once
#include "../core/Config.hpp"
#include "../core/SaveData.hpp"

class Health {
private:
    int currentHealth;
    int maxHealth;
public:
    Health();
    Health(int health);
    ~Health();
    HealthSaveData getHealthSavedata();
    void setHealthSavedata(HealthSaveData data);
    void regenerate(int amount);
    void reduseCurrentHealth(int damage);
    void restoreHealth();
    void setMaxHealth(int newMaxHealth);
    void updateHealth(int str);
    int getCurrentHealth() const;
    int getMaxHealth() const;

};
