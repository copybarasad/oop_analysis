#ifndef OOP_CONSOLE_INPUT_PROCESSOR_H
#define OOP_CONSOLE_INPUT_PROCESSOR_H
#include <memory>

class Command;


class ConsoleInputProcessor {
public:
    std::unique_ptr<Command> process_input();
};


#endif //OOP_CONSOLE_INPUT_PROCESSOR_H