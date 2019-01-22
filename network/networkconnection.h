#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H

#define MAXSIZE 1024

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <list>

class NetworkConnection : public std::enable_shared_from_this<NetworkConnection>
{
public:
    static std::shared_ptr<NetworkConnection> Init(boost::asio::io_context &io);
    void start();
    void end();

    boost::asio::ip::tcp::socket &socket();

private:
    void print(const std::string &data) const;
    NetworkConnection(boost::asio::io_context &io);
    void handle_read(const boost::system::error_code &ec);
    void handle_wait_read(const boost::system::error_code &ec);
    void handle_write(const boost::system::error_code& ec, const std::string &data);

    boost::asio::ip::tcp::socket m_socket;
    std::list<std::string> m_readData;
};

#endif // NETWORKCONNECTION_H
