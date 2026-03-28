#ifndef GAME_ERRORS_HPP_INCLUDED
#define GAME_ERRORS_HPP_INCLUDED

#include <stdexcept>

struct FileOpenError : public std::runtime_error { using std::runtime_error::runtime_error; };
struct ParseError : public std::runtime_error { using std::runtime_error::runtime_error; };
struct ValidationError : public std::runtime_error { using std::runtime_error::runtime_error; };
struct GameLogicError : public std::runtime_error { using std::runtime_error::runtime_error; };
struct RestartGame { };

#endif

