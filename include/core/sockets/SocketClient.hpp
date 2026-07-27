#ifndef YAKSHA_SOCKETCLIENT_H
#define YAKSHA_SOCKETCLIENT_H
#include <string>
#include <vector>

#include "logging/LogManager.hpp"
#include "logging/logger/Logger.hpp"


class SocketClient
{
public:
    explicit SocketClient(const std::string& socket_path);
    ~SocketClient() = default;

    void connectToServer();
    std::vector<std::string> receive();
private:
    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<SocketClient>();

    std::string socket_path;
    int32_t socket_fd;

    std::string buffered_tail;
};


#endif //YAKSHA_SOCKETCLIENT_H