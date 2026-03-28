#ifndef PLAYER_H
#define PLAYER_H

#include "IPlayerCharacter.h"
#include "Health.h"
#include "Attack.h"
#include "Attributes.h"
#include "Equipment.h"
#include "SpellHand.h"
#include "SpellEnhancer.h"
#include "PlayerResources.h"

class Player : public IPlayerCharacter {
private:
	Attributes player_attributes;
	Attack player_attack;
	Equipment player_equipment;
	Health player_health;
	bool slowed;
	SpellHand spell_hand;
	SpellEnhancer spell_enhancer;
	PlayerResources player_resources;

public:
	Player(int intel, int dex, int str, int base_attack, int hp, 
	       int melee_multi, int range_multi, size_t hand_size = 5);
	
	EntityType getType() const override;
	void causeDamage(int damage) override;
	int getDamage() const override;
	std::pair<int, int> getHealth() const override;
	bool alive() const override;
	bool melee() const override;
	void setWeapon() override;
	int getInt() const override;
	int getDex() const override;
	int getStr() const override;
	
	bool isSlowed() const override;
	void setSlow(bool slow_state) override;
	
	std::unique_ptr<IEntity> clone() const override;
	
	SpellHand& getSpellHand();
	const SpellHand& getSpellHand() const;
	SpellEnhancer& getSpellEnhancer();
	const SpellEnhancer& getSpellEnhancer() const;
	
	PlayerResources& getResources();
	const PlayerResources& getResources() const;
	
	void improveIntelligence(int amount);
	void improveDexterity(int amount);
	void improveStrength(int amount);
	void increaseMaxHealth(int amount);
	void restoreHealth();
	int calculateSpellPower() const;
	
	const Attributes& getAttributes() const;
	const Health& getHealthStatus() const;
	
	SpellHand* getSpellHandPtr() override { return &spell_hand; }
	const SpellHand* getSpellHandPtr() const override { return &spell_hand; }
	
	SpellEnhancer* getSpellEnhancerPtr() override { return &spell_enhancer; }
	const SpellEnhancer* getSpellEnhancerPtr() const override { return &spell_enhancer; }
	
	PlayerResources* getResourcesPtr() override { return &player_resources; }
	const PlayerResources* getResourcesPtr() const override { return &player_resources; }
};

#endif 
