#include "input.hpp"
#include <cctype>
#include <limits>
#include <stdexcept>

Input::Input(std::istream& in, std::ostream& out) : in_(in), out_(out) {}

std::string Input::Trim(const std::string& s) {
    std::size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
    return s.substr(a, b - a);
}

std::string Input::ReadNonEmpty(const std::string& prompt, std::size_t max_len) {
    for (;;) {
        out_ << prompt << " ";
        out_.flush();
        std::string line;
        if (!std::getline(in_, line)) {
            throw std::runtime_error("Input stream closed while reading string");
        }
        line = Trim(line);
        if (!line.empty() && line.size() <= max_len) return line;
        out_ << "Invalid input. Please enter a non-empty string up to " << max_len << " characters.\n";
    }
}

int Input::ReadIntRange(const std::string& prompt, int min_val, int max_val) {
    for (;;) {
        out_ << prompt << " [" << min_val << ".." << max_val << "]: ";
        out_.flush();
        std::string line;
        if (!std::getline(in_, line)) {
            throw std::runtime_error("Input stream closed while reading integer");
        }
        line = Trim(line);
        try {
            std::size_t idx = 0;
            long long v = std::stoll(line, &idx, 10);
            if (idx != line.size()) throw std::invalid_argument("Trailing chars");
            if (v >= min_val && v <= max_val) return static_cast<int>(v);
        } catch (...) {
        }
        out_ << "Invalid number. Try again.\n";
    }
}

double Input::ReadDoubleRange(const std::string& prompt, double min_val, double max_val) {
    for (;;) {
        out_ << prompt << " [" << min_val << ".." << max_val << "]: ";
        out_.flush();
        std::string line;
        if (!std::getline(in_, line)) {
            throw std::runtime_error("Input stream closed while reading double");
        }
        line = Trim(line);
        try {
            std::size_t idx = 0;
            double v = std::stod(line, &idx);
            if (idx != line.size()) throw std::invalid_argument("Trailing chars");
            if (v >= min_val && v <= max_val) return v;
        } catch (...) {
        }
        out_ << "Invalid number. Try again.\n";
    }
}

std::string Input::ReadLine(const std::string& prompt) {
    out_ << prompt;
    if (!prompt.empty() && prompt.back() != ' ') {
        out_ << ' ';
    }
    out_.flush();
    std::string line;
    if (!std::getline(in_, line)) {
        throw std::runtime_error("Input stream closed while reading line");
    }
    return Trim(line);
}
