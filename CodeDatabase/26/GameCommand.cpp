#include "GameCommand.hpp"

GameCommand GameCommand::make(Type type) noexcept {
	return GameCommand(type);
}

GameCommand GameCommand::makeTargeted(Type type, const Position& target) noexcept {
	return GameCommand(type, target);
}

GameCommand::Type GameCommand::type() const noexcept {
	return type_;
}

bool GameCommand::hasTarget() const noexcept {
	return hasTarget_;
}

const Position& GameCommand::target() const noexcept {
	return target_;
}

GameCommand::GameCommand(Type type) noexcept
	: type_(type),
	  hasTarget_(false),
	  target_{0, 0} {}

GameCommand::GameCommand(Type type, const Position& target) noexcept
	: type_(type),
	  hasTarget_(true),
	  target_(target) {}
