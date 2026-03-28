#include "command.h" 

Command::Command(CommandType type) : type_(type) {

}

CommandType Command::getType() const {
    return type_;
}