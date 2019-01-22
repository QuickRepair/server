#include "networkserver.h"
#include "networkconnection.h"
#include <boost/bind.hpp>
#include <iostream>

using std::cout;
using std::endl;
using std::pair;
using std::shared_ptr;
using std::make_shared;

using boost::bind;
using boost::asio::ip::tcp;
using boost::asio::io_service;
using boost::asio::ip::address;
using boost::system::error_code;
using boost::asio::placeholders::error;

NetworkServer::NetworkServer()
    :io_service()
{
    m_acceptor=make_shared<tcp::acceptor>(*this, tcp::endpoint(tcp::v4(), PORT));
}

void NetworkServer::start()
{
    handle_acceptor();
    run();
}

void NetworkServer::end()
{

}

void NetworkServer::handle_acceptor()
{
    auto conn=NetworkConnection::Init(*this);
    m_acceptor->async_accept(conn->socket(), bind(&NetworkServer::handle_wait_acceptor, this, error, conn));
}

void NetworkServer::handle_wait_acceptor(const error_code &ec, const shared_ptr<NetworkConnection> &conn)
{
    if(!ec){
        conn->start();
        handle_acceptor();
    }
    else{
        cout<<"handle_wait_acceptor: "<<ec.message()<<endl;
    }
}
