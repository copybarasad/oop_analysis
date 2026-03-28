#include "Event.h"
#include <stdexcept>

Event::Event(EventType type) : type_(type) {}

void Event::addFloatArg(float arg) {
    floatArgs_.push_back(arg);
}

void Event::addIntArg(int arg) {
    intArgs_.push_back(arg);
}

void Event::addStringArg(const std::string& arg) {
    stringArgs_.push_back(arg);
}

float Event::getFloatArg(size_t index) const {
    if (index >= floatArgs_.size()) {
        throw std::out_of_range("Float argument index out of range");
    }
    return floatArgs_[index];
}

int Event::getIntArg(size_t index) const {
    if (index >= intArgs_.size()) {
        throw std::out_of_range("Int argument index out of range");
    }
    return intArgs_[index];
}

const std::string& Event::getStringArg(size_t index) const {
    if (index >= stringArgs_.size()) {
        throw std::out_of_range("String argument index out of range");
    }
    return stringArgs_[index];
}

