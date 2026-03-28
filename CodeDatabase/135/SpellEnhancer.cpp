#include "SpellEnhancer.h"

SpellEnhancer::SpellEnhancer() : enhancement_count(0) {}

void SpellEnhancer::addEnhancement() {
	enhancement_count++;
}

int SpellEnhancer::applyAndReset() {
	int count = enhancement_count;
	enhancement_count = 0;
	return count;
}

int SpellEnhancer::getCount() const {
	return enhancement_count;
}
