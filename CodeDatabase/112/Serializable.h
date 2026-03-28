#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <string>
#include <fstream>

class Serializable {
public:
    virtual ~Serializable() = default;
    
    virtual void serialize(std::ostream& stream) const = 0;
    virtual void deserialize(std::istream& stream) = 0;
    
    virtual std::string getType() const = 0;
};

#endif
