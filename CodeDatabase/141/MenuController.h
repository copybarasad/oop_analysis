#pragma once

#include "MenuTypes.h"
#include <string>

class MenuController {
public:
    MenuController(int argc, char **argv);

    void run();

private:
    int argc;
    char **argv;

    std::shared_ptr<ILogSink> chooseSink(std::string &logFile, bool &fileMode) const;

    int readInt(const std::string &prompt) const;

    std::string selectSlot(const std::vector<std::string> &slots) const;

    int selectUpgrade(const std::vector<UpgradeOption> &options) const;
};
