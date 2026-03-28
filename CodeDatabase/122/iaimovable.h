#ifndef IAIMOVABLE_H
#define IAIMOVABLE_H

#include <utility>
#include "imovable.h"

class IAIMovable : public IMovable
{
public:
   virtual std::pair<int, int> calculateMovement(int chaserX, int chaserY, int targetX, int targetY) = 0;
};

#endif // IAIMOVABLE_H
