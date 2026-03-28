#include "InputFields.hpp"

InputFields::InputFields (std::pair<size_t, size_t> target, std::string extra) :
    target(target), extra(extra) {}

std::pair<size_t, size_t> InputFields::getTarget () const { return target; }

std::string InputFields::getExtra () const { return extra; }