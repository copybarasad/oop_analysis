#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <nlohmann/json_fwd.hpp>

#include "Action.h"


class Game;

using json = nlohmann::json;

template<typename InputImpl>
class InputHandler {
public:
    explicit InputHandler(Game &game, const std::filesystem::path &filePath) : game_(game), input_(), file_(filePath) {
    }

    bool init();


    void tick() {
        input_.tick();
    }

    void get_mouse_pos(int &x, int &y) const {
        input_.get_mouse_pos(x, y);
    }


    std::map<Action, std::string> load() const;

private:
    Game &game_;
    InputImpl input_;
    std::filesystem::path file_;
};

