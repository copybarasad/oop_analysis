#include "PauseConverter.h"
#include "Commands/Rendering/ChangeTheme.h"
#include "Commands/System/ChangeGame.h"
#include "Commands/System/ChangeState.h"


PauseConverter::PauseConverter(Game* game) : game(game) {}


Command* PauseConverter::convert(const InputEvent& event)
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
		switch (event.key)
		{
		case KeyCode::ESC:	return new ChangeState(game, ChangeState::State::PLAY);
		case KeyCode::Num1:	return new ChangeTheme(game, GameTheme::DEFAULT);
		case KeyCode::Num2:	return new ChangeTheme(game, GameTheme::WINTER);
		case KeyCode::Num3: return new ChangeTheme(game, GameTheme::DISERT);
		case KeyCode::S:	return new ChangeGame(game, ChangeGame::Change::SAVEGAME);
		case KeyCode::R:	return new ChangeGame(game, ChangeGame::Change::LOADGAME);
		case KeyCode::Q:	return new ChangeState(game, ChangeState::State::MENU);
		default:
			return nullptr;
		}
	}

	return nullptr;
}