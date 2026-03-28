#include "directdamagespell.h"
#include "player.h"
#include "spell_context.h"
#include "damageable.h"
#include <iostream>

DirectDamageSpell::DirectDamageSpell(int damage, int range) 
	: damage(damage), range(range), temporaryEnhancement(0), permanentEnhancement(0) {}

bool DirectDamageSpell::cast(SpellContext& context) {
	Player& player = context.getPlayer();
	
	if (!player.consumeMana(getManaCost())) {
		std::cout << "Not enough mana for " << getName() << " Spell!\n";
		return false;
	}
	
	int currentRange = range + temporaryEnhancement + permanentEnhancement;
	
	std::vector<SpellTarget> targets = context.getTargetsInRange(currentRange);
	
	if (targets.empty()) {
		std::cout << "No targets in range!\n";
		player.restoreMana(getManaCost());
		return false;
	}
	
	context.displayTargets(targets);
	
	int choice = -1;
	if (!context.trySelectTarget(targets, choice)) {
		std::cout << "Invalid choice!\n";
		player.restoreMana(getManaCost());
		return false;
	}
	
	bool damageApplied = applyDamageToTarget(context, targets[choice], damage);
	
	if (damageApplied) {
		temporaryEnhancement = 0;
		return true;
	}
	
	player.restoreMana(getManaCost());
	temporaryEnhancement = 0;
	return false;
}

bool DirectDamageSpell::applyDamageToTarget(SpellContext& context, const SpellTarget& target, int damageAmount) {
	Damageable* targetObject = target.targetObject;
	if (!targetObject || !targetObject->isAlive()) {
		return false;
	}
	
	targetObject->takeDamage(damageAmount);
	
	// Сущность только информирует о событии, не вызывая напрямую логирование
	context.notifySpellDamage(getName(), target.name, damageAmount, 
	                          targetObject->getHealth(), targetObject->getMaxHealth());
	
	if (!targetObject->isAlive()) {
		context.handleTargetDefeat(*targetObject);
		context.notifySpellTargetDefeated(target.name);
	}
	
	return true;
}


std::unique_ptr<Spell> DirectDamageSpell::clone() const {
	return std::make_unique<DirectDamageSpell>(*this);
}

void DirectDamageSpell::enhance(int enhancementLevel) {
	temporaryEnhancement += enhancementLevel;
}

void DirectDamageSpell::applyPermanentEnhancement(int enhancementLevel) {
	permanentEnhancement += enhancementLevel;
}

int DirectDamageSpell::getEnhancementLevel() const {
	return temporaryEnhancement + permanentEnhancement;
}