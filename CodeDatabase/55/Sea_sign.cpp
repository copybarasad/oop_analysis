#include "Sea_sign.h"
#include <iomanip>


Sea_sign::Sea_sign() : sign('~'), flag(common_flag) {}

Flag Sea_sign::getFlag() {
    return flag;
}

bool Sea_sign::isEmpty() {
    return flag == common_flag;
}

void Sea_sign::setFlag(Flag new_flag) {
    flag = new_flag;
}

void Sea_sign::setSign(Flag choise) {
    switch(choise) {
        case common_flag:
            sign = '~';
            break;
        case player_flag:
            sign = 'i';
            break;
        case enemy_flag:
            sign = 'O';
            break;
        case vortex_flag:
            sign = '@';
            break;
        case fish_net_flag:
            sign = '#';
            break;
        case octopus_flag:
            sign = '&';
            break;
        default : sign = '?'; break;
    }
}

void Sea_sign::setSign(char choise){
    sign = choise;
}

char Sea_sign::getSign() {
    return sign;
}

void Sea_sign::saveTo(SaveFileRAII &out) const{
    out.write(sign).write(static_cast<int>(flag));
}

void Sea_sign::loadFrom(SaveFileRAII &in){
    in.read(sign).read(flag);
}