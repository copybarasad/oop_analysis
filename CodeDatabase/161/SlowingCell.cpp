#include "SlowingCell.h"

MoveResult SlowingCell::EnterEntity(std::shared_ptr<Entity> entity) {
	if (entity->IsStatic())
		return MoveResult::Blocked;

	if (this->entity_ == nullptr) {
		this->entity_ = entity;
		entity->SetNotMoveNext(true);

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

std::shared_ptr<Cell> SlowingCell::cloneStructure() const {
	return std::make_shared<SlowingCell>();
}

std::string SlowingCell::GetName() const {
	return "Slowing";
}