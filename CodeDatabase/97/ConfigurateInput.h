#ifndef UNTITLED_CONFIGURATEINPUT_H
#define UNTITLED_CONFIGURATEINPUT_H


#include "InputHandler.h"
#include <unordered_map>
#include <string>

class ConfigurateInput {
public:
    explicit ConfigurateInput(const std::string& cfgFile = "");

    Command readCommand();

private:
    void loadConfig(const std::string& path);
    void setDefault();
    bool validate() const;

private:
    std::unordered_map<char, CommandType> keyToCommand_;
};


#endif // UNTITLED_CONFIGURATEINPUT_H

