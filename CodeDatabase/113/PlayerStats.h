#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H

class PlayerStats {
private:
    int m_health;
    int m_maxHealth;
    int m_mana;
    int m_maxMana;
    int m_score;

public:
    PlayerStats(int health, int mana) :
        m_health(health),
        m_maxHealth(health),
        m_mana(mana),
        m_maxMana(mana),
        m_score(0)
    {}

    void takeDamage(int amount) {
        m_health = (m_health - amount < 0) ? 0 : m_health - amount;
    }

    void heal(int amount) {
        m_health = (m_health + amount > m_maxHealth) ? m_maxHealth : m_health + amount;
    }

    int getHealth() const { return m_health; }
    int getMaxHealth() const { return m_maxHealth; }
    bool isAlive() const { return m_health > 0; }

    bool spendMana(int amount) {
        if (m_mana >= amount) {
            m_mana -= amount;
            return true;
        }
        return false;
    }

    void addMana(int amount) {
        m_mana = (m_mana + amount > m_maxMana) ? m_maxMana : m_mana + amount;
    }

    int getMana() const { return m_mana; }
    int getMaxMana() const { return m_maxMana; }

    void addScore(int points) { m_score += points; }
    int getScore() const { return m_score; }

    bool validateState() const {
        return m_health >= 0 && m_health <= m_maxHealth &&
            m_mana >= 0 && m_mana <= m_maxMana &&
            m_score >= 0;
    }
};

#endif // PLAYERSTATS_H