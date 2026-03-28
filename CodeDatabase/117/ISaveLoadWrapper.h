#ifndef ISAVELOADWRAPPER_H
#define ISAVELOADWRAPPER_H

#include "../libs/json.hpp"

class ISaveLoadWrapper {
public:
    virtual void save(nlohmann::json& output) = 0;
    virtual void load(const nlohmann::json& input) = 0;
    virtual size_t getHash() const = 0;
    virtual ~ISaveLoadWrapper() = default;
};

#endif