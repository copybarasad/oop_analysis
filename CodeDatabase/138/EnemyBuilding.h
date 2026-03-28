#ifndef ENEMYBUILDING_H
#define ENEMYBUILDING_H

class EnemyBuilding {
public:
	EnemyBuilding(int spawnInterval, int x, int y, int health = 100);
	
	int GetX() const;
	int GetY() const;
	int GetHealth() const;
	bool IsAlive() const;
	void TakeDamage(int damage);
	
	bool ShouldSpawnEnemy() const;
	int GetSpawnInterval() const;
	int GetCurrentCounter() const;
	
	void Update();
	void ResetSpawnCounter();

private:
	int spawnIntervalTurns;
	int turnsUntilNextSpawn;
	int positionX;
	int positionY;
	int currentHealth;
	int maximumHealth;
	
	void validateInvariant() const;
};

#endif
