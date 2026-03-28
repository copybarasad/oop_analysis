#include "exceptions.hpp"

InvalidSignatureError::InvalidSignatureError() : m_Msg("invalid signature") {}
InvalidSignatureError::InvalidSignatureError(const char *msg) : m_Msg(msg) {}

const char *InvalidSignatureError::what() const noexcept {
    return m_Msg.c_str();
}
