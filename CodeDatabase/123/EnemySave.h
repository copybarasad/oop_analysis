#ifndef ENEMY_SAVE
#define ENEMY_SAVE

#include "ISave.h"
#include "../GameSystem/Enemy.h"

class EnemySave : public ISave
{
private:
    shared_ptr<Enemy> *ent;

public:
    EnemySave(shared_ptr<Enemy> *buff)
    {
        ent = buff;
    }

    void write(fstream &file) override
    {
        file.write(reinterpret_cast<char *>(&*(ent->get())), sizeof(Enemy));
    }

    void read(fstream &file) override
    {
        *ent = make_shared<Enemy>();
        file.read(reinterpret_cast<char *>(&*(ent->get())), sizeof(Enemy));
        if (ent->get()->getPosition().x >= 25 || ent->get()->getPosition().y >= 25 || ent->get()->getPosition().x < 0 || ent->get()->getPosition().y < 0)
            throw logic_error("File is broken");
    }
};

#endif