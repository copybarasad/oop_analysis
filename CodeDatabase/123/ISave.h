#ifndef ISAVE
#define ISAVE

#include <fstream>
// #include "../Entity.h"
using namespace std;

class ISave
{

public:
    virtual void write(fstream &) = 0;

    virtual void read(fstream &) = 0;
};

#endif