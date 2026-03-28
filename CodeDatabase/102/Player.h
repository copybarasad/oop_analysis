#pragma once
#include <string>
#include "../Entities/Movable.h"
#include "../Containers/MortalContainer.h"
#include "../Entities/ISwitchingDamager.h"

class Player : public Movable, public IMortal, public ISwitchingDamager
{
    const MortalContainer* _container;
    int _health;
    int _maxHealth;
    int _damage;
    int _sourceNearDamage;
    int _sourceFarDamage;
    int _currentModeIndex = 0;
protected:
    void OnMoveSuccess(Cell target) override;

    void OnMoveFailure(MoveResult result) override;
public:
    struct PlayerData
    {
        Cell _position;
        int _speed;
        int _maxHealth;
        int _nearDamage;
        int _farDamage;

        explicit PlayerData(const Player* player);

        explicit PlayerData(std::string& s);

        std::string ToString() const;
    };

    Player(const MortalContainer* container, Cell position, int speed, int maxHealth, int nearDamage, int farDamage);

    void TakeDamage(int damage) override;

    void SwitchDamageMode(int index) override;

    void ChangeDamageByValue(int value) override;

    int GetHealth() override;

    int GetMaxHealth() override;

    int GetCurrentModeIndex() const override;

    int GetDamage() override;

    bool IsAlive() override;
};