#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <memory>
#include <boost/asio.hpp>
#include "../include/Logger.h"
#include "../include/NetworkProvider.h"

#include <functional>
#include <set>

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

    void register_session(std::shared_ptr<Session> session);
    void unregister_session(std::shared_ptr<Session> session);

private:
    //void sendDataNetwork(const std::vector<char>& message, std::function<void>& callback);
    void startAccepting();
    void stopAccepting();
    void closeSessions();

    boost::asio::ip::tcp::acceptor acceptor;
    std::shared_ptr<Logger> logger;
    boost::asio::io_context& io_context;

    std::set<std::shared_ptr<Session>> sessions;
};