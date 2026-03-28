#ifndef ICOOLDOWNABLE_H
#define ICOOLDOWNABLE_H

class ICooldownable {
public:
	virtual ~ICooldownable() = default;
	
	virtual bool canAttack() const = 0;
	virtual void reduceCooldown() = 0;
	virtual void resetCooldown() = 0;
};

#endif
