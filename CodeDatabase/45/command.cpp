#include "command.hpp"
Command::Command(std::string text) : text(text) {

}

std::string Command::get_text() {
    return text;
}

