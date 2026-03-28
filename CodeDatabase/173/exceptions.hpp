#ifndef SAVE_EXCEPTIONS_HPP
#define SAVE_EXCEPTIONS_HPP

#include <exception>
#include <string>

class InvalidSignatureError : public std::exception {
  public:
    InvalidSignatureError();
    InvalidSignatureError(const char *msg);
    const char *what() const noexcept override;

  private:
    std::string m_Msg;
};

#endif /* SAVE_EXCEPTIONS_HPP */
