#pragma once
#include "NC_Input.h"
#include "Control.h"

char NC_Input::get_input(){
    char c = getch();
    return c;
}
