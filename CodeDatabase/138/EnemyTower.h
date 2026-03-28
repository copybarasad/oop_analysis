#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

class GameField;

class EnemyTower {
public:
	EnemyTower(int x, int y, int attackRadius, int damage, int cooldownTurns);
	
	int GetX() const;
	int GetY() const;
	int GetAttackRadius() const;
	int GetDamage() const;
	int GetCooldownTurns() const;
	int GetCurrentCooldown() const;
	bool CanAttack() const;
	
	void Update(GameField& field);
	void Attack(GameField& field);

private:
	int positionX;
	int positionY;
	int attackRadius;
	int damage;
	int cooldownTurns;
	int turnsUntilAttack;
	
	void validateInvariant() const;
};

#endif
