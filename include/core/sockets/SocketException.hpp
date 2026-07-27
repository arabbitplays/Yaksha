#ifndef YAKSHA_SOCKETEXCEPTION_H
#define YAKSHA_SOCKETEXCEPTION_H

#include <stdexcept>
#include <string>

class SocketException : public std::runtime_error
{
public:
    explicit SocketException(const std::string& message) : std::runtime_error(message) {}
};

#endif //YAKSHA_SOCKETEXCEPTION_H
