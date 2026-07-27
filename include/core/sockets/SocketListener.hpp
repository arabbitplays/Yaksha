#ifndef YAKSHA_SOCKETLISTENER_H
#define YAKSHA_SOCKETLISTENER_H
#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "SocketClient.hpp"

class SocketListener
{
    public:
    explicit SocketListener(const std::string& socket_path);
    ~SocketListener() = default;

    void run();
    void close();
    std::vector<std::string> receiveMessages();

private:
    void listenWorker();

    SocketClient client;
    std::thread listener_thread;

    std::atomic<bool> running{false};
    std::mutex message_mtx;

    std::vector<std::string> pending_messages{};
};


#endif //YAKSHA_SOCKETLISTENER_H