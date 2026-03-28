#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
    private:
    std::string name;

    public:
    Item (std::string name);
    virtual ~Item() = default;
    
    std::string getName () const;
};

#endif