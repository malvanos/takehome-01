#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <functional>

#include "../include/Logger.h"

class NetworkServer;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(
        boost::asio::ip::tcp::socket socket, 
        std::shared_ptr<Logger> logger,
        std::shared_ptr<NetworkServer> server
    );
    ~Session();
    void start();
    void close();
    void send(const std::vector<char>& message, std::function<void(void)>&& callback);
private:
    void read();
    boost::asio::ip::tcp::socket socket;
    std::shared_ptr<Logger> logger;
    std::shared_ptr<NetworkServer> serverAcceptor;

    char data[16] = { 0 };
};