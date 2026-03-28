//
// Created by Artem on 22.10.2025.
//

#ifndef LABAOOP2_NOENTITYINCELLNOTIFICATION_H
#define LABAOOP2_NOENTITYINCELLNOTIFICATION_H

#include <string>
#include "../UniversalStringException.h"

class NoEntityInCellNotification  : public UniversalStringException{
public:
    explicit NoEntityInCellNotification(const std::string &message) : UniversalStringException(message) {}
};


#endif //LABAOOP2_NOENTITYINCELLNOTIFICATION_H
