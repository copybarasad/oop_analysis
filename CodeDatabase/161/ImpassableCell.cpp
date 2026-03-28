#include "ImpassableCell.h"

ImpassableCell::ImpassableCell() : Cell(nullptr) {}

MoveResult ImpassableCell::EnterEntity(std::shared_ptr<Entity>){
	return MoveResult::Unreal;
}

bool ImpassableCell::SetEvent(std::shared_ptr<IEvent> event) {
	return false;
}

std::shared_ptr<Cell> ImpassableCell::cloneStructure() const {
	return std::make_shared<ImpassableCell>();
}

std::string ImpassableCell::GetName() const {
	return "Impassable";
}