#include "buttons.h"


Buttons::Buttons()
{

    QPushButton* btn = new QPushButton();
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setIcon(QIcon("C:/Users/tomil/Downloads/lightning.jpg"));
    btn->setIconSize(QSize(30, 30));
    btn->setText("Light");
    btn->setStyleSheet("QPushButton {"
                       "    border: 1px solid gray;"  // оставить тонкую рамку
                       "    background: white;"       // белый фон
                       "    padding: 0px;"           // убрать отступы
                       "    margin: 0px;"
                       "}"
                       "QPushButton:hover {"
                       "    background: lightgray;"   // подсветка при наведении
                       "}");

    btn->setProperty("spellName", "0");
    btn->setGeometry(0, FIELD_HEIGHT * DOT_HEIGHT, 75, 50);


    QPushButton* btn1 = new QPushButton();
    btn1->setFocusPolicy(Qt::NoFocus);
    btn1->setIcon(QIcon("C:/Users/tomil/Downloads/trap.jpg"));
    btn1->setIconSize(QSize(30, 30));
    btn1->setText("Trap");
    btn1->setStyleSheet("QPushButton {"
                        "    border: 1px solid gray;"
                        "    background: white;"
                        "    padding: 0px;"
                        "    margin: 0px;"
                        "}"
                        "QPushButton:hover {"
                        "    background: lightgray;"
                        "}");

    btn1->setProperty("spellName", "1");
    btn1->setGeometry(80, FIELD_HEIGHT * DOT_HEIGHT, 75, 50);


    QPushButton* btn2 = new QPushButton();
    btn2->setFocusPolicy(Qt::NoFocus);
    btn2->setIcon(QIcon("C:/Users/tomil/Downloads/poison.jpg"));
    btn2->setIconSize(QSize(30, 30));
    btn2->setText("Poison");
    btn2->setStyleSheet("QPushButton {"
                        "    border: 1px solid gray;"
                        "    background: white;"
                        "    padding: 0px;"
                        "    margin: 0px;"
                        "}"
                        "QPushButton:hover {"
                        "    background: lightgray;"
                        "}");

    btn2->setProperty("spellName", "2");
    btn2->setGeometry(160, FIELD_HEIGHT * DOT_HEIGHT, 75, 50);


    QPushButton* btn3 = new QPushButton();
    btn3->setFocusPolicy(Qt::NoFocus);
    btn3->setIcon(QIcon("C:/Users/tomil/Downloads/leon.jpg"));
    btn3->setIconSize(QSize(30, 30));
    btn3->setText("Leon");
    btn3->setStyleSheet("QPushButton {"
                        "    border: 1px solid gray;"
                        "    background: white;"
                        "    padding: 0px;"
                        "    margin: 0px;"
                        "}"
                        "QPushButton:hover {"
                        "    background: lightgray;"
                        "}");

    btn3->setProperty("spellName", "3");
    btn3->setGeometry(240, FIELD_HEIGHT * DOT_HEIGHT, 75, 50);


    QPushButton* btn4 = new QPushButton();
    btn4->setFocusPolicy(Qt::NoFocus);
    btn4->setIcon(QIcon("C:/Users/tomil/Downloads/improve.jpg"));
    btn4->setIconSize(QSize(25, 25));
    btn4->setText("Improve");
    btn4->setStyleSheet("QPushButton {"
                        "    border: 1px solid gray;"
                        "    background: white;"
                        "    padding: 0px;"
                        "    margin: 0px;"
                        "}"
                        "QPushButton:hover {"
                        "    background: lightgray;"
                        "}");

    btn4->setProperty("spellName", "4");
    btn4->setGeometry(320, FIELD_HEIGHT * DOT_HEIGHT, 75, 50);


    myButtons.push_back(btn);
    myButtons.push_back(btn1);
    myButtons.push_back(btn2);
    myButtons.push_back(btn3);
    myButtons.push_back(btn4);
}

std::vector<QPushButton *> &Buttons::get_my_buttons()
{
    return myButtons;
}


void Buttons::set_color(int number, int color)
{
    QString colorStr;

    switch(color) {
    case 0: colorStr = "white"; break;      // можно купить
    case 1: colorStr = "lightgreen"; break; // в руке
    case 2: colorStr = "yellow"; break; // нельзя купить
    case 3: colorStr = "lightred"; break;  // нет места
    case 4: colorStr = "lightblue"; break;  // выбрано
    default: colorStr = "white"; break;
    }
    myButtons[number]->setStyleSheet(QString(
                                         "QPushButton {"
                                         "    border: 1px solid gray;"
                                         "    background: %1;"
                                         "    padding: 0px;"
                                         "    margin: 0px;"
                                         "}"
                                         "QPushButton:hover {"
                                         "    background: lightgray;"
                                         "}").arg(colorStr));
}

void Buttons::updateVisibility(int currentOption) {

    for (QPushButton* btn : myButtons) {
        if (btn) {
            btn->setVisible(currentOption == 2);
            btn->raise();
        }
    }
}
