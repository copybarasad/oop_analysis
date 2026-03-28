#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H

#include "IInput.h"
#include <string>

class ConsoleInput : public IInput {
public:
    ConsoleInput() = default;
    Command nextCommand() override;
private:
    Command parseLine(const std::string &s) const;
};

#endif // CONSOLEINPUT_H
