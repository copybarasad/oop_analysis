#include "PlayConverter.h"
#include "Commands/GameMechanics/Spell/SpellCommnad.h"
#include "Commands/GameMechanics/Movement/Move.h"
#include "Commands/GameMechanics/Movement/ChangeSpeed.h"
#include "Commands/GameMechanics/Attack/Attack.h"
#include "Commands/GameMechanics/Attack/ChangeAttackMode.h"
#include "Commands/System/ChangeGame.h"
#include "Commands/System/ChangeState.h"
#include "Commands/Configuration/KeyConfiguration.h"
#include "Interfaces/Game.h"
#include <iostream>


PlayConverter::PlayConverter(Game* game) : game(game)
{
	config = new KeyConfiguration(game->getKeyConfigPath());
}


Command* PlayConverter::convert(const InputEvent& event)
{
	if (event.device == InputDevice::System)
	{
		switch (event.systemEvent)
		{
		case SystemEventType::WindowClosed:	return new ChangeGame(game, ChangeGame::Change::EXITGAME);
		default:
			return nullptr;
		}
	}
	
	if (event.device == InputDevice::Keyboard)
	{
		switch (config->getKey(event.key))
		{
		case KeyCode::ESC:	return new ChangeState(game, ChangeState::State::PAUSE);
		//spells
		case KeyCode::Num1:	return new SpellCommand(game, 0);
		case KeyCode::Num2:	return new SpellCommand(game, 1);
		case KeyCode::Num3:	return new SpellCommand(game, 2);
		case KeyCode::Num4:	return new SpellCommand(game, 3);
		case KeyCode::Num5:	return new SpellCommand(game, 4);
		case KeyCode::Z:	return new SpellCommand(game, SpellType::AreaDamageSpell);
		case KeyCode::X:	return new SpellCommand(game, SpellType::DirectDamageSpell);
		case KeyCode::C:	return new SpellCommand(game, SpellType::TrapSpell);
		case KeyCode::V:	return new SpellCommand(game, SpellType::AllySpell);
		case KeyCode::B:	return new SpellCommand(game, SpellType::UpgradeSpell);
		case KeyCode::M:	return new SpellCommand(game, { 0, 0 });
		//move
		case KeyCode::LShift:	return new ChangeSpeed(game);
		case KeyCode::Left:		return new Move(game, Move::Direction::LEFT);
		case KeyCode::Right:	return new Move(game, Move::Direction::RIGHT);
		case KeyCode::Up:		return new Move(game, Move::Direction::UP);
		case KeyCode::Down:		return new Move(game, Move::Direction::DOWN);
		//attack
		case KeyCode::L:	return new ChangeAttackMode(game);
		case KeyCode::A:	return new Attack(game, Attack::Direction::LEFT);
		case KeyCode::D:	return new Attack(game, Attack::Direction::RIGHT);
		case KeyCode::W:	return new Attack(game, Attack::Direction::UP);
		case KeyCode::S:	return new Attack(game, Attack::Direction::DOWN);

		case KeyCode::Q:	return new ChangeGame(game, ChangeGame::Change::EXITGAME);
		default:
			std::cout << u8"Клавиша не распознана.\n";
			return nullptr;
		}
	}

	if (event.device == InputDevice::Mouse)
	{
		switch (event.mouseButton)
		{
		case MouseButton::Left:
			return new SpellCommand(game, { event.x, event.y });
		default:
			return nullptr;
		}
	}

	return nullptr;
}


PlayConverter::~PlayConverter()
{
	if (config != nullptr)
	{
		delete config;
		config = nullptr;
	}
}