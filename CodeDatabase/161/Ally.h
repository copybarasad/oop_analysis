#ifndef ALLY_H
#define ALLY_H

#include "Entity.h"

class Ally : public Entity{
public:
	Ally();
	Ally(int);
	bool IsFriendly() const override;
	TokenEntity serialise() const;
};



#endif //ALLY_H
