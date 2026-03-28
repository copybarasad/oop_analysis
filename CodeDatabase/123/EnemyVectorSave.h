#ifndef ENEMY_VECTOR_SAVE
#define ENEMY_VECTOR_SAVE

#include "ISave.h"
#include "EnemySave.h"
#include "../GameSystem/Map.h"

class EnemyVectorSave : public ISave
{
private:
    std::vector<std::shared_ptr<Enemy>> *mEnemyList;

public:
    EnemyVectorSave(std::vector<std::shared_ptr<Enemy>> *list)
    {
        mEnemyList = list;
    }

    void write(fstream &file) override
    {
        EnemySave *sv;
        int b = mEnemyList->size();
        file.write(reinterpret_cast<char *>(&b), sizeof(int));
        for (size_t i = 0; i < mEnemyList->size(); i++)
        {
            sv = new EnemySave(&mEnemyList->operator[](i));
            sv->write(file);
            delete sv;
        }
    }

    void read(fstream &file) override
    {
        mEnemyList->clear();
        EnemySave *sv;
        int b = 0;
        file.read(reinterpret_cast<char *>(&b), sizeof(int));
        mEnemyList->resize(b);
        for (int i = 0; i < b; i++)
        {
            // mEnemyList->at(i) = std::make_shared<Enemy>();
            sv = new EnemySave(&mEnemyList->at(i));
            sv->read(file);
            delete sv;
        }
    }
};

#endif