#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <nlohmann/json.hpp>
#include "FileModel.h"
#include "SaveLoadException.h"

class FileController {
public:
    void saveJson(const std::string& filename, const nlohmann::json& data) const;
    nlohmann::json loadJson(const std::string& filename, bool checkHash=true) const;
};

#endif

