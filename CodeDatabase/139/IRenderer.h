#pragma once
#include "DataTypes/GameThemes.h"


class IRenderer
{
public:
	virtual void renderMap() = 0;
	virtual void renderPlayInfo() = 0;
	virtual void renderMenu() = 0;
	virtual void renderPause() = 0;
	virtual void renderGameWin() = 0;
	virtual void renderGameOver() = 0;
	virtual void renderLevelWin() = 0;
	virtual void renderLevelBetween() = 0;
	virtual void setTheme(GameTheme theme) = 0;
};