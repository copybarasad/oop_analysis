#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include "field4ver.h"
#include "spell1ver.h"
#include <iostream>
#include <iomanip> 

class Renderer {
public:
    virtual ~Renderer() = default;
    
    virtual void print_field(Field& field) = 0;
    virtual void print_hero(Hero& hero, unsigned int win_points) = 0;
    virtual void print_hand(Hand& hand) = 0;
    virtual void print_message(const std::string& message) = 0;
    virtual void print_menu(std::vector<std::string>& options) = 0;
};

class Console_renderer : public Renderer {
public:
    void print_hero(Hero& hero, unsigned int win_points) override;

    void print_hand(Hand& hand) override;

    void print_field(Field& field) override;

    void print_message(const std::string& message) override;
    
    void print_menu(std::vector<std::string>& options) override;
};

#endif