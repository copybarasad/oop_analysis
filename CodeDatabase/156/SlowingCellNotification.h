//
// Created by Artem on 15.10.2025.
//

#ifndef LABAOOP2_SLOWINGCELLNOTIFICATION_H
#define LABAOOP2_SLOWINGCELLNOTIFICATION_H

#include <string>
#include "../UniversalStringException.h"

class SlowingCellNotification : public UniversalStringException {
public:
    explicit SlowingCellNotification(const std::string &message) : UniversalStringException(message) {}
};


#endif //LABAOOP2_SLOWINGCELLNOTIFICATION_H
