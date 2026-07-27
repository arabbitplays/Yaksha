#include "../../../include/core/sockets/SocketServer.hpp"

#include <sys/socket.h>
#include <sys/un.h>

#include "core/sockets/SocketException.hpp"

SocketServer::SocketServer(std::string socket_path) : socket_path(socket_path)
{
}

void SocketServer::create()
{
    server = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server < 0)
    {
        throw SocketException("Failed to create socket: " + std::string(strerror(errno)));
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socket_path.c_str());
    unlink(addr.sun_path);

    if (bind(server, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        throw SocketException("Failed to bind socket to path " + socket_path + ":" + std::string(strerror(errno)));
    }

    if (listen(server, 5) < 0)
    {
        throw SocketException("Failed to listen on socket: " + std::string(strerror(errno)));
    }

    logger->info("Listening on socket " + socket_path);
}

int32_t SocketServer::acceptClient()
{
    return accept(server, nullptr, nullptr);
}

std::string SocketServer::receive(int32_t client)
{
    char buf[256];
    int n = read(client, buf, sizeof(buf) - 1);
    if (n > 0)
    {
        buf[n] = 0;
        return std::string(buf);
    }
    return "";
}

void SocketServer::send(int32_t client, const std::string& message) const
{
    const char* data = message.c_str();
    size_t remaining = message.size();
    while (remaining > 0)
    {
        ssize_t written = write(client, data, remaining);
        if (written < 0)
        {
            if (errno == EINTR) continue;
            logger->error("Write failed: " + std::string(strerror(errno)));
            break;
        }
        data += written;
        remaining -= written;
    }
    shutdown(client, SHUT_WR); // signal that the write has finished
    logger->debug("Sent message:\n" + message);
}

void SocketServer::closeClient(int32_t client)
{
    close(client);
}