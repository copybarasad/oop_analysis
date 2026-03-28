#ifndef UTILS_INPUT_HPP
#define UTILS_INPUT_HPP

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Input {
  public:
    Input();
    Input &quitOnEOF(std::string msg);
    Input &withPrompt(std::string prompt);
    Input &withOption(std::string opt);
    Input &withReject(std::string reject);
    std::string collect();

  private:
    std::string m_Prompt;
    std::string m_Reject;
    std::vector<std::string> m_Options;
    bool m_QuitOnEOF;
    std::string m_QuitMsg;

    bool inputIsValid(std::string input);
    void printPrompt();
    void printReject();
    void printQuit();
};

#endif /* UTILS_INPUT_HPP */
