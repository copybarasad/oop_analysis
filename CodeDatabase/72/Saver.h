//
// Created by Mac on 10.11.2025.
//

#ifndef GAME_TERM_SAVER_H
#define GAME_TERM_SAVER_H
#include <fstream>
#include <iosfwd>


class Saver {

    std::ofstream file;


public:

    explicit Saver(const std::string & filename);

    ~Saver();

    template<typename T>
    void save(const T &data) {
        file.write(reinterpret_cast<const char*>(&data), sizeof(T));
    }
};


#endif //GAME_TERM_SAVER_H