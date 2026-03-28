#ifndef BUILDING_SAVE
#define BUILDING_SAVE

#include "ISave.h"
#include "../GameSystem/Building.h"

class BuldingSave : public ISave
{
private:
    shared_ptr<Building> *ent;

public:
    BuldingSave(shared_ptr<Building> *buff)
    {
        ent = buff;
    }

    void write(fstream &file) override
    {
        file.write(reinterpret_cast<char *>(&*(ent->get())), sizeof(Building));
    }

    void read(fstream &file) override
    {
        *ent = make_shared<Building>(5, 5);
        file.read(reinterpret_cast<char *>(&*(ent->get())), sizeof(Building));
        if (ent->get()->getPosition().x >= 25 || ent->get()->getPosition().y >= 25 || ent->get()->getPosition().x < 0 || ent->get()->getPosition().y < 0)
            throw logic_error("File is broken");
    }
};

#endif