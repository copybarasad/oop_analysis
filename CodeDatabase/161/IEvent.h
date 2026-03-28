#ifndef IEVENT_H
#define IEVENT_H

#include <iostream>

#include "../../../entites/Entity.h"

enum class EventType {
	None,
	Trap,
};


class IEvent {
public:
	virtual ~IEvent() = default;
	virtual EventType getType() = 0;
	virtual void activateIvent(std::shared_ptr<Entity>) = 0;

};


#endif //IEVENT_H
