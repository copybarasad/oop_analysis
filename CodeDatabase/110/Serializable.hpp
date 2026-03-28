#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <string>
#include <sstream>

class Serializable {
    public:
    virtual ~Serializable() = default;
    
    virtual std::string serialize() const = 0;
};

#endif