#ifndef INPUT_HPP
#define INPUT_HPP

#include <istream>
#include <ostream>
#include <string>

class Input {
public:
    Input(std::istream& in, std::ostream& out);

    std::string ReadNonEmpty(const std::string& prompt, std::size_t max_len = 128);

    int ReadIntRange(const std::string& prompt, int min_val, int max_val);

    double ReadDoubleRange(const std::string& prompt, double min_val, double max_val);

    std::string ReadLine(const std::string& prompt);

private:
    std::istream& in_;
    std::ostream& out_;

    std::string Trim(const std::string& s);
};

#endif
