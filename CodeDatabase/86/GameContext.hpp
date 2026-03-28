#pragma once

#include "DisplayManager.hpp"
#include "InputManager.hpp"
#include "Logger.hpp"

struct GameContext {
  Logger logger_;
  InputManager inputManager;
  DisplayManager displayManager;
};
