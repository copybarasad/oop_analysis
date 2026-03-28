#include "input.hpp"

Input::Input()
    : m_Prompt(">> "), m_Reject(""), m_QuitOnEOF(false), m_QuitMsg("") {}

Input &Input::withPrompt(std::string prompt) {
    m_Prompt = prompt;
    return *this;
}

Input &Input::withOption(std::string opt) {
    m_Options.push_back(opt);
    return *this;
}

Input &Input::withReject(std::string reject) {
    m_Reject = reject;
    return *this;
}

Input &Input::quitOnEOF(std::string msg) {
    m_QuitOnEOF = true;
    m_QuitMsg = msg;
    return *this;
}

std::string Input::collect() {
    std::string line;

    while (true) {
        printPrompt();

        if (!std::getline(std::cin, line)) {
            if (m_QuitOnEOF) {
                printQuit();
                std::exit(0);
            }
            throw std::runtime_error("received EOF");
        }

        if (inputIsValid(line)) {
            return line;
        }

        printReject();
    }
}

bool Input::inputIsValid(std::string input) {
    if (m_Options.empty()) {
        return true;
    }

    return std::find(m_Options.begin(), m_Options.end(), input) !=
           m_Options.end();
}

void Input::printPrompt() {
    if (!m_Prompt.empty()) {
        std::cout << m_Prompt;
    }
}

void Input::printReject() {
    if (!m_Reject.empty()) {
        std::cout << m_Reject << std::endl;
    }
}

void Input::printQuit() {
    if (!m_QuitMsg.empty()) {
        std::cout << m_QuitMsg << std::endl;
    }
}
