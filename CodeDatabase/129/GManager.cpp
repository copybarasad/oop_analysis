#include "GManager.h"

GManager::GManager() {}
GManager::GManager(int height, int width, int hand_size, int level) {
    manager = Manager(height, width, hand_size, level);
}

GManager::GManager(int height, int width, std::vector<std::string> spells, int level) {
    manager = Manager(height, width, spells, level);
}
GManager::~GManager() {}



void GManager::start_work(){
    initscr(); //активируем окно терминала
    noecho(); //чтобы вводимые символы не отображались
    keypad(stdscr, TRUE); //включаем считывание клавиш
    refresh(); //обновляем
}


bool GManager::work() {
    bool condition = true;
    while(condition) {
        // отрисовка
        visualizer.draw(manager);

        // ввод команды + действие
        condition = !manager.check_condition(manager.get_level()) && 
                    controller.process(manager); 
    }
    // выполнено ли условие
    condition = manager.check_condition(manager.get_level());
    return condition;
}


void GManager::end_work(bool pass){
    HelpDisplay hd;
    clear(); //очищаем наше окошко
    if(!pass) hd.print_big_game_over(); //если мы не прошли игру - рисуем game over
    if(pass) hd.print_win(); //если прошли - рисуем победу
    getch(); //type any key to exit
    endwin(); //закрываем окошко
    return;
}



std::vector<std::string> GManager::get_reset_spells(){
    return manager.get_reset_spells();
}

//для сохранения и загрузки
Saving GManager::serialize() const{
    Saving result = manager.serialize();
    return result;
}

void GManager::deserialize(const Saving& data){
    manager.deserialize(data);
}
