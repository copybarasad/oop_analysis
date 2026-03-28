//
// Created by Artem on 24.09.2025.
//

#ifndef LABA1_UNIVERSALSTRINGEXCEPTION_H
#define LABA1_UNIVERSALSTRINGEXCEPTION_H

class UniversalStringException : public std::exception{
public:
    UniversalStringException(const std::string& message): message{message}{}

    const char* what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};


#endif //LABA1_UNIVERSALSTRINGEXCEPTION_H
