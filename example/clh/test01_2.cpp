#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <iostream>
#include <boost/asio/basic_waitable_timer.hpp>

using namespace boost::asio;

int main()
{
  io_service ioservice;

  steady_timer timer{ioservice, std::chrono::seconds{3}};
  std::cout << "begin to wait timer\n";
  timer.wait();   // block
  std::cout << "3 sec\n";

//  timer.async_wait([](const boost::system::error_code &ec)
//    { std::cout << "3 sec\n"; });

  ioservice.run();
}