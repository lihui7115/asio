#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <array>
#include <string>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

io_service ioservice;
tcp::resolver resolv{ioservice};
tcp::socket tcp_socket{ioservice};
std::array<char, 4096> bytes;

void read_handler(const boost::system::error_code &ec,
  std::size_t bytes_transferred)
{
  if (!ec)
  {
    std::cout.write(bytes.data(), bytes_transferred);
    tcp_socket.async_read_some(buffer(bytes), read_handler);
  }
}

void connect_handler(const boost::system::error_code &ec)
{
  if (!ec)
  {
//    std::string r =
//      "GET http://theboostcpplibraries.com/ HTTP/1.1\r\n"
//      "Host: theboostcpplibraries.com\r\n"
//      "Proxy-Authorization: Basic YmFycnkueHU6c29ueWJhcnJ5\r\n"
//      "User-Agent: asio/0.0.1\r\n"
//      "Accept: */*\r\n"
//      "Proxy-Connection: Keep-Alive\r\n\r\n";
    std::string r =
      "GET / HTTP/1.1\r\nHost: theboostcpplibraries.com\r\n\r\n";
    write(tcp_socket, buffer(r));
    tcp_socket.async_read_some(buffer(bytes), read_handler);
  }
}

void resolve_handler(const boost::system::error_code &ec,
  tcp::resolver::iterator it)
{
  if (!ec)
    tcp_socket.async_connect(*it, connect_handler);
}

int main()
{
  // not to support http_proxy  ( TODO. test at home )

  // another way to test, set /etc/hosts for the "theboostcpplibraries.com" with a local server
  // and then it will output the local server http information
  tcp::resolver::query q{"theboostcpplibraries.com", "80"};
  resolv.async_resolve(q, resolve_handler);
  ioservice.run();
}