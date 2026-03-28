//
// Created by Mac on 11.11.2025.
//

#ifndef GAME_TERM_ISERIALIZABLE_H
#define GAME_TERM_ISERIALIZABLE_H
#include "Reader.h"
#include "Saver.h"


class ISerializable {
public:
    virtual void save(Saver& saver) const = 0;
    virtual void load(Reader& reader) = 0;
    virtual ~ISerializable() = default;
};


#endif //GAME_TERM_ISERIALIZABLE_H