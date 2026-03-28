#pragma once

#include "SaveLoadException.h"

class FileWriteException : public SaveLoadException {
 public:
  explicit FileWriteException(const std::string& filename);
};
