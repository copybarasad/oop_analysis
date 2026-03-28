#include "Command.h"

Command::Command() : valid(true) {
}

bool Command::isValid() const {
	return valid;
}

