#pragma once


class Entity {
protected:
	int damage;
	int health;
	int stunDuration;
public:
	Entity(int healthPoints, int attackDamage);
	Entity() = default;
	virtual ~Entity();

	Entity(const Entity&) = default;
    Entity(Entity&&) noexcept = default;
    Entity& operator=(const Entity&) = default;
    Entity& operator=(Entity&&) noexcept = default;

	void TakeDamage(int amount);
	int GetDamage() const;
	bool IsAlive() const;
	int GetHealth() const;

	void SetStunDuration(int duration);
	int GetStunDuration() const;
	bool IsStunned() const;
	void DecreaseStunDuration();
};
