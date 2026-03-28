#include "plot.hpp"

Plot::Plot(char type) : type(type), originalType(type), obj(nullptr), ownsCharacter(false) {}

Plot::Plot(char type, Character* obj) : type(type), originalType(type), obj(obj), ownsCharacter(false) {}


Plot::~Plot() {
    if (ownsCharacter) {
        delete obj;
    }
}

void Plot::placeObj(Character* newObj, bool takeOwnership) {
    if (obj == newObj) return;
    
    if (ownsCharacter) {
        delete obj;
    }
    
    obj = newObj;
    ownsCharacter = takeOwnership;
    
    if (obj != nullptr) {
        type = obj->getIcon();
    } else {
        if (type == 'P' || type == 'E' || type == 'T' || type == 'M') {
            type = '*'; 
        }
    }
}

void Plot::removeObj() {
    if (ownsCharacter) {
        delete obj;
    }
    obj = nullptr;
    ownsCharacter = false;
    
    type = originalType;
}

bool Plot::isNotPassable() {
    return type == '#';
}

bool Plot::isEmpty() {
    return obj == nullptr && !isNotPassable(); 
}

void Plot::setTrap(){
    type = 'x';  
}

bool Plot::isTrap(){
    return type == 'x';
}

void Plot::consumeChest(){
    if (originalType == 'G') {
        originalType = '*';
        if (type == 'G') {
            type = '*';
        }
    }
}

bool Plot::isChest(){
    return type == 'G';
}