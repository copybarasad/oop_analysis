#include "ObjectFactory.h"

Object *ObjectFactory::create(const std::string &objName) {
    auto it = this->creators_.find(objName);
    if (it != this->creators_.end()) {
        Object* obj = it->second();
        return obj;
    }
    return nullptr;
}