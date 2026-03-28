#ifndef LEVEL_SAVE
#define LEVEL_SAVE

#include "ISave.h"

class LevelSave : public ISave
{
private:
    size_t *mLevel;

public:
    LevelSave(size_t *lvl)
    {
        mLevel = lvl;
    }

    void write(fstream &file) override
    {
        file.write(reinterpret_cast<char *>(&*mLevel), sizeof(size_t));
    }

    void read(fstream &file) override
    {
        file.read(reinterpret_cast<char *>(&*mLevel), sizeof(size_t));
    }
};

#endif