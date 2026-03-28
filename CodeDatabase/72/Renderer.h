//
// Created by bob on 10/14/25.
//

#ifndef GAME_TERM_RENDERER_H
#define GAME_TERM_RENDERER_H
#include "../field_objects/Field.h"


class Renderer {

private:
    void printColored(const std::string &text, const std::string &color);
public:
    void drawField(const Field &field);
};


#endif //GAME_TERM_RENDERER_H