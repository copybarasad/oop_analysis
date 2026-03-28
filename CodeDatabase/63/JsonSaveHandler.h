#ifndef RPG_JSONSAVEHANDLER_H
#define RPG_JSONSAVEHANDLER_H

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class JsonSaveHandler {
    std::fstream file;
public:
    explicit JsonSaveHandler(const std::string &filename);

    ~JsonSaveHandler();

    bool save(const json& data);
};

#endif