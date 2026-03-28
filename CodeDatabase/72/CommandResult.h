//
// Created by Mac on 02.12.2025.
//

#ifndef GAME_TERM_COMMANDRESULT_H
#define GAME_TERM_COMMANDRESULT_H
#include <string>

struct CommandResult {
    bool shouldExit = false;
    bool shouldSave = false;
    bool moveEnemies = true;
    std::string message = "";

    static CommandResult success(const std::string& msg = "") {
        return {false, false, true, msg};
    }

    static CommandResult exit(bool save = false) {
        return {true, save, false, "Exiting game..."};
    }

    static CommandResult failure(const std::string& msg) {
        return {false, false, false, msg};
    }
};

#endif //GAME_TERM_COMMANDRESULT_H