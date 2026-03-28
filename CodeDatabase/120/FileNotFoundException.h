#pragma once

#include "SaveLoadException.h"

class FileNotFoundException : public SaveLoadException {
 public:
  explicit FileNotFoundException(const std::string& filename);
};
