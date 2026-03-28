#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

class EnemyBuilding {
public:
    EnemyBuilding(int x, int y, int spawn_interval, int health);

    int GetX() const;
    int GetY() const;
    int GetHealth() const;
    int GetMaxHealth() const;
    int GetSpawnInterval() const;
    int GetTurnsSinceLastSpawn() const;

    bool ShouldSpawnEnemy();
    void TakeDamage(int damage);
    bool IsAlive() const;
    void IncreaseStats(int health_bonus);
    void SetTurnsSinceLastSpawn(int turns);

private:
    int x_;
    int y_;
    int health_;
    int max_health_;
    int spawn_interval_;
    int turns_since_last_spawn_;
};

#endif // ENEMY_BUILDING_H
