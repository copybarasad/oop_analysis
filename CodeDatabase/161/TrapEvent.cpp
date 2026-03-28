#include "TrapEvent.h"

TrapEvent::TrapEvent(int damage) : damage_(std::max(damage, 10)) {};

TrapEvent::TrapEvent() : TrapEvent(10) {};

void TrapEvent::activateIvent(std::shared_ptr<Entity> entity){
	entity->CauseDamage(damage_);
}

EventType TrapEvent::getType(){
	return EventType::Trap;
}