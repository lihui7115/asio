#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <iostream>
#include <boost/asio/basic_waitable_timer.hpp>
#include <thread>
#include <mutex>

using namespace boost::asio;
using namespace std::placeholders;

// just a test, so declare in the global
std::mutex mutex;

void timer_alarm(const boost::system::error_code &ec, steady_timer& timer, const char* name) {
  if (!ec) {       // error_code has a "bool operation()" that checking ec.value() not equal 0 inside
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "thread_id:" << std::this_thread::get_id() << " timer_name:" << name << ": alarm 1 sec\n";
  } else {
    std::cout << "error time alarm:"  << ec.value() << std::endl;
    return;
  }

  // restart need to do the following thing ?
  timer.expires_from_now(std::chrono::seconds{1});   // better to calculate the accurate elapsed time
  timer.async_wait(std::bind(&timer_alarm, _1, std::ref(timer), name));
}

int main()
{
  io_service ioservice;

  std::cout << "begin to wait timer\n";
  steady_timer timer{ioservice, std::chrono::seconds{1}};
  timer.async_wait(std::bind(&timer_alarm, _1, std::ref(timer), "timer1"));

  steady_timer timer2{ioservice, std::chrono::seconds{1}};
  timer2.async_wait(std::bind(&timer_alarm, _1, std::ref(timer2), "timer2"));

  steady_timer timer3{ioservice, std::chrono::seconds{1}};
  timer3.async_wait(std::bind(&timer_alarm, _1, std::ref(timer3), "timer3"));

  std::thread thread1{[&ioservice](){ ioservice.run(); }};
  std::thread thread2{[&ioservice](){ ioservice.run(); }};
  thread1.join();
  thread2.join();

  // ioservice.run();   // run inside the thread

  // run() returns if there are no pending asynchronous operations.
}