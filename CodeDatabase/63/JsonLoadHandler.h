#ifndef RPG_JSONLOADHANDLER_H
#define RPG_JSONLOADHANDLER_H

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class JsonLoadHandler {
    std::fstream file;
public:
    explicit JsonLoadHandler(const std::string &filename);

    ~JsonLoadHandler();

    json load();
};


#endif