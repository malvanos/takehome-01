#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <memory>
#include <boost/asio.hpp>
#include "../include/Logger.h"

class NetworkServer
{
public:
    struct Dependencies {
        boost::asio::io_context& io_context;
        std::shared_ptr<Logger> logger;
    };

    NetworkServer(Dependencies&& deps);
    ~NetworkServer();
    void start();
    void stop();
private:
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    std::shared_ptr<Logger> logger;
};