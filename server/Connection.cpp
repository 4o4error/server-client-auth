#include "connection.hpp"
#include <utility>
#include <vector>
#include <iostream>

namespace http {
  namespace server {
    asio::ip::tcp::socket& connection::getSocket(){
      return socket_;
    }
    connection::connection(asio::io_service& io_service)
      : socket_(io_service)
    {
    }

    void connection::start()
    {
      do_read();
    }

    void connection::stop()
    {
    //  socket_.close();
    }

    void connection::do_read()
    {
      socket_.async_read_some(asio::buffer(buffer_),
        [this](std::error_code ec, std::size_t bytes_transferred)
      {
        if (!ec)
        {
          std::cout << "SUCCESS" << std::endl;
          do_write();
        }

      });
    }

    void connection::do_write()
    {
      /*
      const std::string head = " HTTP / 1.1 200 OK\r\n \
                                                          Content - Type: text/html; charset = utf - 8\r\n \
                                                                                     Content - Length:";
      std::stringstream ss;
      std::string message = "<html><head></head><body> hello</body></html>";
      ss << head << message.length() << "\r\n" << "\r\n\r\n" << message;
      
      */
      char data_[1024] = {'a','b'};
      
      socket_.async_write_some(asio::buffer(data_), [this](std::error_code ec, std::size_t){
        asio::error_code ignored_ec;
        stop();
      });
    }
   

  } // namespace server
} // namespace http