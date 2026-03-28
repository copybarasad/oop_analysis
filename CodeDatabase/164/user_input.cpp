#include "user_input.h"

std::array<unsigned int, 2> User_input::input_coords(unsigned int height, unsigned int width){
    std::cout << "Enter coordinates:\n";
    std::string input;
    char char_input;
    unsigned int int_input;
    while(true){
        std::cout << "(input symbol and number) ";
        std::getline(std::cin, input);
        std::istringstream iss(input);
        if(iss >> char_input >> int_input){
            if('a' <= char_input && char_input < 'a' + width && 
                    0 < int_input && int_input <= height){
                unsigned int x = char_input - 'a';
                std::array<unsigned int, 2> coords = {x, int_input - 1};
                return coords;
            }
            std::cout << "coordinates out of field\n";
        }
    }
};

std::array<unsigned int, 2> User_input::input_coords_in_range(unsigned int x_base, unsigned int y_base, 
        unsigned int height, unsigned int width, unsigned int range){
    std::array<unsigned int, 2> coords;
    while(true){
        coords = input_coords(height, width);
        if(0 < abs((int)x_base - (int)coords[0]) + abs((int)y_base - (int)coords[1]) && 
                abs((int)x_base - (int)coords[0]) + abs((int)y_base - (int)coords[1]) <= range){
            return coords;
        }
        std::cout << "too far away\n";
    }
};

unsigned int User_input::input_int(){
    std::cout << "Enter number:\n";
    std::string input;
    unsigned int int_input;
    while(true){
        std::cout << "(input number) ";
        std::getline(std::cin, input);
        std::istringstream iss(input);
        if(iss >> int_input){
            return int_input;
        }
    }
};

char User_input::input_char(){
    std::string input;
    char char_input;
    while(true){
        std::cout << "(input symbol) ";
        std::getline(std::cin, input);
        std::istringstream iss(input);
        if(iss >> char_input) return char_input;
    }
};

void User_input::input_enter(){
    std::cout <<"Press enter to continue...";
    char c;
    do{
        std::cin.get(c);
    }while(c != '\n');
};