#include "networkconnection.h"
#include "jsonAnalyser.h"
#include <boost/bind.hpp>
#include <iostream>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::endl;
using std::list;
using std::string;
using std::shared_ptr;
using std::make_shared;

using boost::bind;
using boost::split;
using boost::asio::buffer;
using boost::asio::ip::tcp;
using boost::asio::io_context;
using boost::system::error_code;
using boost::is_any_of;
using boost::token_compress_on;
using boost::asio::placeholders::error;

NetworkConnection::NetworkConnection(io_context &io)
    :m_socket{io}
{

}

void NetworkConnection::handle_read(const error_code& ec)
{
    if(!ec){
        string data="";
        for(auto &str:m_readData){
            data+=str;
        }
        m_readData.clear();
        list<string> results;
        split(results, data, is_any_of("\r\n"), token_compress_on);
        m_readData.push_back(string(results.back()));
        m_socket.async_wait(tcp::socket::wait_read, bind(&NetworkConnection::handle_wait_read, shared_from_this(), error));
        results.pop_back();
        for(auto &result:results)
            m_socket.async_wait(tcp::socket::wait_write, bind(&NetworkConnection::handle_write, shared_from_this(), error, string(result)));
    }
    else{
        cout<<"@read: "<<ec.message()<<endl;
        end();
    }
}

void NetworkConnection::handle_wait_read(const error_code &ec)
{
    if(!ec){
        string data(MAXSIZE, 0);
        m_socket.async_read_some(buffer(data), bind(&NetworkConnection::handle_read, shared_from_this(), error));
        m_readData.push_back(data);
    }
    else{
        cout<<"@wait_read: "<<ec.message()<<endl;
        end();
    }
}

void NetworkConnection::handle_write(const error_code &ec, const string &data)
{
    if(!ec){
        string result=JsonAnalyser::Init().shunt_message(shared_from_this(), data);
        result+="\r\n";
        m_socket.async_write_some(buffer(result.data(), result.size()), bind(&NetworkConnection::print, shared_from_this(), result));
    }
    else{
        cout<<m_socket.local_endpoint().address()<<"@wait_write: "<<ec.message()<<endl;
    }

}

tcp::socket &NetworkConnection::socket()
{
    return m_socket;
}

void NetworkConnection::print(const std::string &data) const
{
    cout<<"print: "<<data<<endl;
}

shared_ptr<NetworkConnection> NetworkConnection::Init(io_context &io)
{
    return shared_ptr<NetworkConnection>(new NetworkConnection(io));
}

void NetworkConnection::start()
{
    cout<<"start: "<<shared_from_this().use_count()<<endl;
    m_socket.async_wait(tcp::socket::wait_read, bind(&NetworkConnection::handle_wait_read, shared_from_this(), error));
}

void NetworkConnection::end()
{
    cout<<"end: "<<shared_from_this().use_count()<<endl;
}
