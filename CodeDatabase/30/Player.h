#pragma once
#include "../../gamecore/include/Enums.h"
#include "Entity.h"
#include <memory>

// Коэффициент для расчета базового дальнего урона из базового ближнего (если не задан явно)
static constexpr double DMG_COEFF = 0.75;

class Hand;

class Player : public Entity {
public:
    Player(int hp, int baseMeleeDamage);
    ~Player() override;
    int hp() const;
    int damage() const;

    void switchToMelee();
    void switchToRanged();
    AttackMode attackMode() const;
    bool isSlowed() const;
    void setSlowed(bool v);

    // Заклинания
    Hand& hand();
    const Hand& hand() const;
    std::unique_ptr<Hand> takeHand();
    void setHand(std::unique_ptr<Hand> h);

    void giveKillReward(); // вызывается после убийства врага: каждые 3 убийства добавить новый заряд случайного заклинания

    EntityType type() const override;

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

    void upgradeMaxHp(int amount);
    void upgradeDamage(int amount);
    
    int baseDamage() const;
    int maxHp() const;

private:
    int meleeDamage_{};
    int rangedDamage_{};
    int damage_{};
    int maxHp_{};
    AttackMode mode_{AttackMode::Melee};
    bool slowed_{false};
    int killCounter_{0};
    std::unique_ptr<Hand> hand_;
};