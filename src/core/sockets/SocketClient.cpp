#include "../../../include/core/sockets/SocketClient.hpp"

#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "core/sockets/SocketException.hpp"

SocketClient::SocketClient(const std::string& socket_path) : socket_path(socket_path)
{
}

void SocketClient::connectToServer()
{
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    sockaddr_un server_address;
    server_address.sun_family = AF_UNIX;
    std::strcpy(server_address.sun_path, socket_path.c_str());

    connect(socket_fd, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));

    logger->info("Connected to server socket " + socket_path);
}

std::vector<std::string> SocketClient::receive()
{
    char buffer[256];
    std::string pending = buffered_tail;
    std::vector<std::string> lines{};
    buffered_tail = "";
    while (lines.empty())
    {
        ssize_t n = read(socket_fd, buffer, sizeof(buffer));
        if (n == 0)
        {
            throw SocketException("Socket " + socket_path + " closed by peer");
        }
        if (n < 0)
        {
            throw SocketException("read() on socket " + socket_path + " failed: " + std::strerror(errno));
        }
        pending.append(buffer, n);

        // split into lines
        while (true)
        {
            size_t nl = pending.find('\n');
            if (nl != std::string::npos)
            {
                lines.push_back(pending.substr(0, nl));
                pending = pending.substr(nl + 1);
            } else
            {
                break;
            }
        }
    }

    buffered_tail = pending;
    return lines;
}
