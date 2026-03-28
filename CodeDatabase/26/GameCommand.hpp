#pragma once

#include <optional>

#include "Position.hpp"

class GameCommand {
public:
	enum class Type {
		None,
		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,
		AttackUp,
		AttackDown,
		AttackLeft,
		AttackRight,
		SwitchMode,
		CastSpell1,
		CastSpell2,
		CastSpell3,
		CastSpell4,
		CastSpell5,
		SaveGame,
		LoadGame,
		QuitToMenu,
		StartCampaign,
		LoadCampaignFromMenu,
		ExitApplication
	};

	static GameCommand make(Type type) noexcept;
	static GameCommand makeTargeted(Type type, const Position& target) noexcept;

	Type type() const noexcept;
	bool hasTarget() const noexcept;
	const Position& target() const noexcept;

private:
	explicit GameCommand(Type type) noexcept;
	GameCommand(Type type, const Position& target) noexcept;

	Type type_;
	bool hasTarget_;
	Position target_;
};
