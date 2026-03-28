#pragma once

#include <string>

class Field;
class GameWorld;

class GameEvent {
public:
	enum class Type {
		PlayerMoved,
		PlayerAttacked,
		DamageApplied,
		SpellGranted,
		SpellCast,
		AllySummoned,
		StructureDestroyed,
		StateChanged,
		CommandIssued,
		SystemMessage
	};

	static GameEvent makeMessage(Type type, std::string message);
	static GameEvent stateChanged(const Field* field, const GameWorld* world);

	Type type() const noexcept;
	const std::string& message() const noexcept;
	const Field* field() const noexcept;
	const GameWorld* world() const noexcept;

private:
	GameEvent(Type type, std::string message, const Field* field, const GameWorld* world);

	Type type_;
	std::string message_;
	const Field* field_;
	const GameWorld* world_;
};
