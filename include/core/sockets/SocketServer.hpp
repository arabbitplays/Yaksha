#ifndef YAKSHA_SOCKETLISTENER_H
#define YAKSHA_SOCKETLISTENER_H
#include <string>

#include "logging/LogManager.hpp"
#include "logging/logger/Logger.hpp"

class SocketServer
{
public:
    explicit SocketServer(std::string socket_path);
    ~SocketServer() = default;

    void create();
    int32_t acceptClient();
    std::string receive(int32_t client);
    void send(int32_t client, const std::string& message) const;
    void closeClient(int32_t client);

private:
    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<SocketServer>();

    std::string socket_path;
    int32_t server;
};


#endif //YAKSHA_SOCKETLISTENER_H