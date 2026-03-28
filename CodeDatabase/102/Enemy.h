#pragma once
#include "../Containers/MortalContainer.h"
#include "../Entities/Movable.h"
#include "../Entities/IDamager.h"

class Enemy : public Movable, public IMortal, public IDamager
{
    const MortalContainer* _container;
    int _health;
    int _maxHealth;
    int _damage;
protected:
    void OnMoveSuccess(Cell target) override;

    void OnMoveFailure(MoveResult result) override;
public:
    struct EnemyData
    {
        Cell _position;
        int _speed;
        int _maxHealth;
        int _damage;

        explicit EnemyData(const Enemy* enemy);

        explicit EnemyData(std::string& s, int index);

        std::string ToString(int index) const;
    };

    Enemy(const MortalContainer* container, Cell position, int speed, int maxHealth, int damage);

    void TakeDamage(int damage) override;

    void ChangeDamageByValue(int value) override;

    int GetHealth() override;

    int GetMaxHealth() override;

    int GetDamage() override;

    bool IsAlive() override;
};