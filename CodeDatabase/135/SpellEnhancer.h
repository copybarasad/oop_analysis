#ifndef SPELL_ENHANCER_H
#define SPELL_ENHANCER_H

class SpellEnhancer {
private:
	int enhancement_count;

public:
	SpellEnhancer();
	
	void addEnhancement();
	int applyAndReset();
	int getCount() const;
};

#endif
