#ifndef TARGET_H
#define TARGET_H

#include "damageable.h"
#include <string>

class Target {
private:
	std::string name;
	Damageable* targetObject;

public:
	Target(const std::string& targetName, Damageable* object);
	
	std::string getName() const;
	int getPositionX() const;
	int getPositionY() const;
	int getCurrentHealth() const;
	int getMaximumHealth() const;
	Damageable* getTargetObject() const;
};

#endif
