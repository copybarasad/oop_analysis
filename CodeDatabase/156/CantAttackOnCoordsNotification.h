//
// Created by Artem on 20.12.2025.
//

#ifndef LABAOOP2_CANTATTACKONCOORDSNOTIFICATION_H
#define LABAOOP2_CANTATTACKONCOORDSNOTIFICATION_H

#include <string>
#include "../UniversalStringException.h"

class CantAttackOnCoordsNotification : public UniversalStringException{
public:
    explicit CantAttackOnCoordsNotification(const std::string &message) : UniversalStringException(message) {}
};


#endif //LABAOOP2_CANTATTACKONCOORDSNOTIFICATION_H
