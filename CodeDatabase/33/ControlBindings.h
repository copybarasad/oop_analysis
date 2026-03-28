#pragma once
#include <map>
#include <set>
#include <string>
#include "Command.h"

class ControlBindings {
public:
    struct Mapping {
        CommandType type = CommandType::Invalid;
        int dx = 0;
        int dy = 0;
    };

    static ControlBindings LoadOrDefault(const std::string& path);

    Command MapInput(char key, const std::string& payload) const;
    bool IsUsingDefault() const;

private:
    ControlBindings() = default;

    static ControlBindings MakeDefault();
    static bool IsValidSet(const std::map<char, Mapping>& candidate);

    std::map<char, Mapping> bindings_;
    bool using_default_ = true;
};
