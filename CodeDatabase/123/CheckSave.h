#ifndef CHECK_SAVE
#define CHECK_SAVE

#include "ISave.h"
#include <string>

#define SAVE_FILE_CHECK "#game"

class CheckSave : public ISave
{
public:
    void write(fstream &file) override
    {
        char check[] = SAVE_FILE_CHECK;
        file.write(check, sizeof(char) * sizeof(SAVE_FILE_CHECK));
    }

    void read(fstream &file) override
    {
        char *check = new char[sizeof(SAVE_FILE_CHECK)];
        file.read(check, sizeof(char) * sizeof(SAVE_FILE_CHECK));
        if (string(check) != string(SAVE_FILE_CHECK))
            throw std::logic_error("This is not a Game's file");
    }
};

#endif