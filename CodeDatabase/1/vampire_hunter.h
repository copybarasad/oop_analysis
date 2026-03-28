#pragma once
#include "character.h"
#include "spell_hand.h"  

class VampireHunter : public Character {
private:
    int research_progress_;  
    int silver_bullets_;    
    int oak_stakes_;        
    bool is_ranged_mode_;    
    bool used_chapel_;
    bool used_armory_;
    std::vector<Position> visited_libraries_;
    SpellHand spellHand_; 
    bool has_ammo_upgrade_ = false; 
    // поля для прокачки 
    int max_health_ = 100;           // для улучшения здоровья
    int attack_damage_bonus_ = 0;    // бонус к урону атаки
    int spell_damage_bonus_ = 0;     // бонус к урону заклинаний

public:
    VampireHunter(const std::string& name, const Position& position);

    void takeDamage(int amount) override;
    void applyCellEffect(CellType cell_type) override; 
  
    void doResearch();
    bool isReadyForFinalBlow() const;
    int getResearchProgress() const;
 
    void switchCombatMode();
    bool isRangedMode() const;
    int getSilverBullets() const;
    int getOakStakes() const;
    void useSilverBullet();
    void useOakStake();
    int getCurrentAttackDamage() const;
    bool canAttackAtPosition(const Position& target) const;
    
    void applyLibraryEffect();
    void applyChapelEffect();
    void applyArmoryEffect();
    void applyVampireLairEffect();
    
    bool hasUsedChapel() const;
    bool hasUsedArmory() const;
    bool hasVisitedLibrary(const Position& pos) const;
    
    SpellHand& getSpellHand() { return spellHand_; }
    const SpellHand& getSpellHand() const { return spellHand_; }

    //lb3

    // НОВЫЕ методы для прокачки
    void upgradeHealth(int amount);
    void upgradeAttackDamage(int amount);
    void upgradeSpellPower(int amount);
    void upgradeAmmo(int bullets, int stakes);
    
    // НОВЫЕ геттеры для сохранения/отображения
    int getMaxHealth() const { return max_health_; }
    int getAttackDamageBonus() const { return attack_damage_bonus_; }
    int getSpellDamageBonus() const { return spell_damage_bonus_; }
    
    // НОВЫЕ сеттеры для загрузки
    void setMaxHealth(int health) { max_health_ = health; }
    void setAttackDamageBonus(int bonus) { attack_damage_bonus_ = bonus; }
    void setSpellDamageBonus(int bonus) { spell_damage_bonus_ = bonus; }
    void setSilverBullets(int count) { silver_bullets_ = count; }
    void setOakStakes(int count) { oak_stakes_ = count; }
    void setRangedMode(bool ranged) { is_ranged_mode_ = ranged; }
    void setResearchProgress(int progress) { research_progress_ = progress; }
    void setAmmoUpgrade(bool has_upgrade) { has_ammo_upgrade_ = has_upgrade; }
    bool hasAmmoUpgrade() const { return has_ammo_upgrade_; }
};