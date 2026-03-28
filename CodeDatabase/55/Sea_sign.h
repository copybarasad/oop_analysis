#pragma once
#include "Enums.h"
#include "Serializable.h"

class Sea_sign {
private:
    char sign;
    Flag flag;
    
public:
    Sea_sign();
    Flag getFlag();
    bool isEmpty();
    void setFlag(Flag new_flag);
    void setSign(Flag choise);
    void setSign(char choise);
    char getSign();
    void saveTo(SaveFileRAII &out) const;
    void loadFrom(SaveFileRAII &in);
};