//
// Created by Mac on 10.11.2025.
//

#ifndef GAME_TERM_READER_H
#define GAME_TERM_READER_H
#include <fstream>
#include <iosfwd>


class Reader {

    std::ifstream file;

public:

    explicit Reader(const std::string & filename);

    ~Reader();

    template <typename T>
    void read(T & data) {
        file.read(reinterpret_cast<char *>(&data), sizeof(T));
    }
};


#endif //GAME_TERM_READER_H