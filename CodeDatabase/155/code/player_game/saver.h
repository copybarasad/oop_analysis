#include <string>
#include <iostream>
#ifndef SAVERH
#define SAVERH

#define DATA_SEP '/'
#define DATA_SEP2 '\n'
#define TILES_LIST "∘▲♥█░♠☗♝◈⋇"

class map;

class game_master;

class saver
{
    public:
        saver();

        void save(const std::string&, const std::string&);

        std::string load(const std::string&);

        std::string data_cipher(map*, game_master*);

        void data_decipher(std::string, game_master*);

        int data_decipher_helper(std::string*, char);

        std::string data_decipher_helper_str(std::string*, char);

        ~saver();
};

#endif