#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <iostream>
#include <boost/asio/basic_waitable_timer.hpp>


using namespace boost::asio;
using namespace std::placeholders;

void timer_alarm(const boost::system::error_code &ec, steady_timer& timer) {
  if (!ec) {       // error_code has a "bool operation()" that checking ec.value() not equal 0 inside
    std::cout << "1 sec\n";
  } else {
    std::cout << "error time alarm:"  << ec.value() << std::endl;
    return;
  }

  // restart need to do the following thing ?
  timer.expires_from_now(std::chrono::seconds{1});   // better to calculate the accurate elapsed time
  timer.async_wait(std::bind(&timer_alarm, _1, std::ref(timer)));
}

int main()
{
  io_service ioservice;

  steady_timer timer{ioservice, std::chrono::seconds{1}};
  std::cout << "begin to wait timer\n";
  timer.async_wait(std::bind(&timer_alarm, _1, std::ref(timer)));
    
  std::cout << "do something else\n";

  ioservice.run();
  
  // run() returns if there are no pending asynchronous operations.
}