#include "../h/Spell.h"
#include <cmath>
#include <utility>

using namespace std;

Spell::Spell(int range, string  name)
	: range(range), name(std::move(name)) {
}


int Spell::getRange() const {
	return range;
}

string Spell::getName() const {
	return name;
}

// Checks if target is within spell range using Manhattan distance
bool Spell::isInRange(const Position& casterPos, const Position& targetPos) const {
	int dx = abs(targetPos.x - casterPos.x);
	int dy = abs(targetPos.y - casterPos.y);
	int distance = dx + dy;
	return distance <= range;
}
