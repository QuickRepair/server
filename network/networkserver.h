#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#define PORT 8888

#include <boost/asio.hpp>
#include <memory>

class NetworkConnection;

class NetworkServer : public boost::asio::io_service
{
public:
    NetworkServer();

    void start();
    void end();

private:
    void handle_acceptor();
    void handle_wait_acceptor(const boost::system::error_code &ec, const std::shared_ptr<NetworkConnection> &conn);
    std::shared_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
};

#endif // NETWORKSERVER_H
