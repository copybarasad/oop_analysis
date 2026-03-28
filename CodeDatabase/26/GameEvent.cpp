#include "GameEvent.hpp"

GameEvent GameEvent::makeMessage(Type type, std::string message) {
	return GameEvent(type, std::move(message), nullptr, nullptr);
}

GameEvent GameEvent::stateChanged(const Field* field, const GameWorld* world) {
	return GameEvent(Type::StateChanged, std::string(), field, world);
}

GameEvent::Type GameEvent::type() const noexcept {
	return type_;
}

const std::string& GameEvent::message() const noexcept {
	return message_;
}

const Field* GameEvent::field() const noexcept {
	return field_;
}

const GameWorld* GameEvent::world() const noexcept {
	return world_;
}

GameEvent::GameEvent(Type type, std::string message, const Field* field, const GameWorld* world)
	: type_(type),
	  message_(std::move(message)),
	  field_(field),
	  world_(world) {}
