#ifndef PLAYER_H
#define PLAYER_H

enum class CombatMode {
    MELEE,
    RANGED
};

class Player {
public:
    Player(int x, int y, int max_health = 100, int melee_damage = 20, int ranged_damage = 12);

    int GetHealth() const;
    int GetMaxHealth() const;
    int GetDamage() const;
    int GetMeleeDamage() const;
    int GetRangedDamage() const;
    int GetScore() const;
    int GetX() const;
    int GetY() const;
    CombatMode GetCombatMode() const;
    bool IsSlowed() const;
    int GetExperiencePoints() const;
    int GetLevel() const;

    void TakeDamage(int damage);
    void Heal(int amount);
    void AddScore(int points);
    void SetPosition(int x, int y);
    void SwitchCombatMode();
    void SetSlowed(bool slowed);
    void AddExperience(int exp);
    void LevelUp();
    void IncreaseMaxHealth(int amount);
    void IncreaseDamage(int melee, int ranged);
    bool IsAlive() const;
    void RestoreHealth();

private:
    int health_;
    int max_health_;
    int melee_damage_;
    int ranged_damage_;
    int score_;
    int x_;
    int y_;
    CombatMode combat_mode_;
    bool is_slowed_;
    int experience_points_;
    int level_;
};

#endif // PLAYER_H
