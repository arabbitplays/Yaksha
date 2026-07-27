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
    if (listener_thread.joinable())
    {
        listener_thread.join();
    }
}

void SocketListener::listenWorker()
{
    try
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
    catch (...)
    {
        std::lock_guard<std::mutex> lock(message_mtx);
        worker_exception = std::current_exception();
        running.store(false);
    }
}

std::vector<std::string> SocketListener::receiveMessages()
{
    std::vector<std::string> result;
    std::exception_ptr pending_exception;
    {
        std::lock_guard<std::mutex> lock(message_mtx);
        result = pending_messages;
        pending_messages.clear();
        pending_exception = worker_exception;
        worker_exception = nullptr;
    }
    if (pending_exception)
    {
        std::rethrow_exception(pending_exception);
    }
    return result;
}
