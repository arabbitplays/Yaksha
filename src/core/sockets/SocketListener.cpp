#include "../../../include/core/sockets/SocketListener.hpp"

SocketListener::SocketListener(const std::string& socket_path) : client(SocketClient(socket_path))
{
    client.connectToServer();
}

void SocketListener::listen()
{
    running.store(true);
    listener_thread = std::thread(&SocketListener::listenWorker, this);
}

void SocketListener::close()
{
    running.store(false);
    listener_thread.join();
}

void SocketListener::listenWorker()
{
    while (running.load())
    {
        std::vector<std::string> lines = client.receive();
        {
            std::lock_guard<std::mutex> lock(message_mtx);
            for (const auto& line : lines)
            {
                pending_messages.emplace_back(line);
            }
        }
    }
}

std::vector<std::string> SocketListener::receiveMessages()
{
    std::vector<std::string> result;
    {
        std::lock_guard<std::mutex> lock(message_mtx);
        result = pending_messages;
        pending_messages.clear();
    }
    return result;
}
