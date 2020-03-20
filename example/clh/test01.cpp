#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <iostream>

using namespace boost::asio;

int main()
{
  io_service ioservice;

  steady_timer timer{ioservice, std::chrono::seconds{3}};
  timer.async_wait([](const boost::system::error_code &ec)
    { std::cout << "3 sec\n"; });

  ioservice.run();
}


/*
 
 # build
 boost/libs/asio/example/clh# b2
 
 # run
 boost/bin.v2/libs/asio/example# export INDEX=01 && ./clh/test${INDEX}.test/gcc-7/debug/threading-multi/visibility-hidden/test${INDEX}
 
 */