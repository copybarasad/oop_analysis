#include "CastSpellCommand.h"

CastSpellCommand::CastSpellCommand(int index, const Position& target)
    : spellIndex(index), targetPosition(target) {}

CommandType CastSpellCommand::getType() const {
    return CommandType::CAST_SPELL;
}

bool CastSpellCommand::isValid() const {
    return spellIndex >= 0;
}

int CastSpellCommand::getSpellIndex() const {
    return spellIndex;
}

Position CastSpellCommand::getTargetPosition() const {
    return targetPosition;
}