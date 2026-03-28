#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include <ostream>
#include <istream>

class Serializable
{
public:
    ~Serializable() = default;
    virtual void serialize(std::ostream& os) const = 0;
    virtual void deserialize(std::istream& is) = 0;
};

#endif // SERIALIZABLE_H
