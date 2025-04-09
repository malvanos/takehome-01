#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <memory>
#include <boost/asio.hpp>
#include "../include/Logger.h"
#include "../include/NetworkProvider.h"

#include <functional>

class NetworkServer : 
    public NetworkProvider,
    public std::enable_shared_from_this<NetworkServer>
{
public:
    struct Dependencies {
        boost::asio::io_context& ioContext;
        std::shared_ptr<Logger> logger;
    };

    NetworkServer(Dependencies&& deps);
    ~NetworkServer() override;
    void start() override;
    void stop() override;
    void send(const std::vector<char>& message, std::function<void>&& callback) override;

private:
    //void sendDataNetwork(const std::vector<char>& message, std::function<void>& callback);

    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    std::shared_ptr<Logger> logger;
    boost::asio::io_context& io_context;
};