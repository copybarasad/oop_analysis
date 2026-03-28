#ifndef PLAYER_ECONOMY_H
#define PLAYER_ECONOMY_H
class PlayerEconomy {
private:
    int score;
    int mana;
    int maxMana;
    int enemiesDefeated;

public:
    PlayerEconomy(int startMana = 100, int maxMana = 100);
    
    int getScore() const;
    int getMana() const;
    int getMaxMana() const;
    int getEnemiesDefeated() const;
    
    void addScore(int pts);
    void addMana(int amount);
    void addEnemyDefeated();
     
    void setScore(int newScore);
    void setMana(int newMana);
    
    bool spendMana(int amount);
    bool spendScore(int amount);
    void setEnemiesDefeated(int count) {
        enemiesDefeated = count;
    }
};

#endif