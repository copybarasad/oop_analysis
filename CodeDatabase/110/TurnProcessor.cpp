#include "TurnProcessor.hpp"

TurnProcessor::TurnProcessor (size_t id, EntityManager& manager) :
    id(id), manager(manager) {}

void TurnProcessor::setID (size_t id) { this->id = id; }