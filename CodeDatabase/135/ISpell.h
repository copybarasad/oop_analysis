#ifndef ISPELL_H
#define ISPELL_H

class SpellContext;

class ISpell {
public:
	virtual bool cast(SpellContext& context) = 0;
	virtual ~ISpell() = default;
};

#endif
