#include "Cell.h"

Cell::Cell(std::shared_ptr<Entity> entity) : entity_(entity) {}

Cell::Cell() : entity_(nullptr) {}

MoveResult Cell::EnterEntity(std::shared_ptr<Entity> entity) {
	if (entity->IsStatic())
		return MoveResult::Blocked;

	if (entity_ == nullptr) {
		entity_ = entity;

		if (event_) {
			event_->activateIvent(entity_);
			event_ = nullptr;
		}

		return MoveResult::Moved;
	}

	if (!entity->CanHit())
		return MoveResult::Blocked;

	if (entity->IsFriendly() != this->entity_->IsFriendly()) {
		this->entity_->CauseDamage(entity->GetDamage());
		return MoveResult::HitEntity;
	}

	return MoveResult::Blocked;
}

bool Cell::RemoveEntity(std::shared_ptr<Entity> entity) {
	if (this->entity_ != entity)
		return false;

	this->entity_ = nullptr;
	return true;
}

bool Cell::SpawnEntity(std::shared_ptr<Entity> entity) {
	if (this->entity_ != nullptr)
		return false;

	this->entity_ = entity;
	return true;
}

bool Cell::IsEmpty() const {
	return this->entity_ == nullptr;
}

bool Cell::EqualEntites(std::shared_ptr<Entity> entity) {
	return entity_ == entity;
}

bool Cell::SetEvent(std::shared_ptr<IEvent> event) {
	if (!event_) {
		event_ = event;
		return true;
	}
	return false;
}

std::shared_ptr<IEvent> Cell::GetEvent() {
	return event_;
}

// Клонируем только пустую структуру клетки
std::shared_ptr<Cell> Cell::cloneStructure() const {
	return std::make_shared<Cell>();
}

std::shared_ptr<Entity> Cell::GetEntity() const {
	return entity_;
}

std::string Cell::GetName() const {
	return "Cell";
}