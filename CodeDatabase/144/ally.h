#ifndef ALLY_H
#define ALLY_H

class AllyActionContext;

class Ally {
private:
    int health;
    int damage;
    int positionX;
    int positionY;
    int movementCooldown;

public:
	struct Snapshot {
		int health = 0;
		int damage = 0;
		int positionX = -1;
		int positionY = -1;
		int movementCooldown = 0;
		bool alive = false;
	};

    Ally(int health, int damage, int x, int y);
    
    void update(AllyActionContext& context);
    int getPositionX() const;
    int getPositionY() const;
    int getHealth() const;
    int getDamage() const;
	int getMovementCooldown() const;
    bool isAlive() const;
    void takeDamage(int damageAmount);
	void setPosition(int x, int y);
	void setHealth(int value);
	void setDamage(int value);
	void setMovementCooldown(int cooldown);
	Snapshot createSnapshot() const;
	static Ally fromSnapshot(const Snapshot& snapshot);

private:
    void moveTowardsNearestEnemy(AllyActionContext& context);
    void attackEnemyInRange(AllyActionContext& context);
    bool isValidMove(const AllyActionContext& context, int x, int y) const;
};

#endif