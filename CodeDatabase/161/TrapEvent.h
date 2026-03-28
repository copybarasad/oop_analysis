#ifndef TRAPEVENT_H
#define TRAPEVENT_H

#include "IEvent.h"

class TrapEvent : public IEvent{
public:
	TrapEvent();
	TrapEvent(int);

	EventType getType() override;
	void activateIvent(std::shared_ptr<Entity>) override;

private:
	int damage_;
};


#endif //TRAPEVENT_H
