//
// Created by Artem on 30.10.2025.
//

#ifndef LABAOOP2_CANTCASTSPELLONCELLNOTIFICATION_H
#define LABAOOP2_CANTCASTSPELLONCELLNOTIFICATION_H

#include <string>
#include "../UniversalStringException.h"

class CantCastSpellOnCellNotification : public UniversalStringException{
public:
    explicit CantCastSpellOnCellNotification(const std::string &message) : UniversalStringException(message) {}
};


#endif //LABAOOP2_CANTCASTSPELLONCELLNOTIFICATION_H
