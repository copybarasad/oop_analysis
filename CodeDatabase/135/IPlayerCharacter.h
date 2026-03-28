#ifndef IPLAYER_CHARACTER_H
#define IPLAYER_CHARACTER_H

#include "ICombatant.h"
#include "ISlowable.h"

class SpellHand;
class SpellEnhancer;
class PlayerResources;
class Attributes;
class Health;

class IPlayerCharacter : public ICombatant, public ISlowable {
public:
	virtual bool melee() const = 0;
	virtual void setWeapon() = 0;
	virtual int getInt() const = 0;
	virtual int getDex() const = 0;
	virtual int getStr() const = 0;
	
	virtual SpellHand* getSpellHandPtr() { return nullptr; }
	virtual const SpellHand* getSpellHandPtr() const { return nullptr; }
	
	virtual SpellEnhancer* getSpellEnhancerPtr() { return nullptr; }
	virtual const SpellEnhancer* getSpellEnhancerPtr() const { return nullptr; }
	
	virtual PlayerResources* getResourcesPtr() { return nullptr; }
	virtual const PlayerResources* getResourcesPtr() const { return nullptr; }
};

#endif
