#ifndef BUTTONS_H
#define BUTTONS_H
#include <vector>
#include <QPushButton>
#include <QLayout>
#include <QWidget>
#include "constants.h"
#include <string>

class Buttons
{

public:
    Buttons();
    std::vector<QPushButton*>& get_my_buttons();
    void set_color(int number, int count);
    void updateVisibility(int currentOption);

private:
    std::vector<QPushButton*> myButtons;

};

#endif // BUTTONS_H
