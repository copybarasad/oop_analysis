#ifndef DAMAGEABLE_H
#define DAMAGEABLE_H

class EnemyActionContext;

class Damageable {
public:
	virtual ~Damageable() = default;
	
	virtual void takeDamage(int damageAmount) = 0;
	virtual int getHealth() const = 0;
	virtual int getMaxHealth() const = 0;
	virtual bool isAlive() const = 0;
	virtual int getPositionX() const = 0;
	virtual int getPositionY() const = 0;
	virtual void onDefeated(EnemyActionContext& context) { (void)context; }
};

#endif
