#pragma once

#include <iostream>

inline int id = 1;

class ButtonModel {
private:
    std::string title_;
    std::function<void()> handler_;
    int id_;
public:
    ButtonModel(std::string title,std::function<void()> handler): 
        title_(title),
        handler_(handler),
        id_(id++) {}

    const std::string getTitle() const { return title_; }

    const int getId() const { return id_; }

    void click() { if(handler_) handler_(); }
};