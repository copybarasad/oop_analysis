#pragma once

#include "ExceptionHandler.h"
#include "FileNotFoundException.h"
#include "FileReadException.h"
#include "InvalidJsonException.h"
#include "json.hpp"
#include "Utils.h"
#include "SpellFactory.h"

class LoadManager {
 public:
  bool LoadGame(const std::string& filename, GameWorld& world);
};
