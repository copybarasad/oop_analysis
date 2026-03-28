#ifndef RPG_VISUALCONTEXT_H
#define RPG_VISUALCONTEXT_H

#include <string>

typedef struct {
    bool changed;
    std::string title;
    std::string message;
} VisualContext;


#endif