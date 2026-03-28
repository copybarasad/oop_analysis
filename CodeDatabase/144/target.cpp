#include "target.h"

Target::Target(const std::string& targetName, Damageable* object)
	: name(targetName), targetObject(object) {}

std::string Target::getName() const { 
	return name; 
}

int Target::getPositionX() const { 
	return targetObject ? targetObject->getPositionX() : -1; 
}

int Target::getPositionY() const { 
	return targetObject ? targetObject->getPositionY() : -1; 
}

int Target::getCurrentHealth() const { 
	return targetObject ? targetObject->getHealth() : 0; 
}

int Target::getMaximumHealth() const { 
	return targetObject ? targetObject->getMaxHealth() : 0; 
}

Damageable* Target::getTargetObject() const { 
	return targetObject; 
}
