#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include <array>
#include <memory>
#include <asio.hpp>

namespace http {
  namespace server {



    /// Represents a single connection from a client.
    class connection
    {
    public:
      connection(const connection&) = delete;
      connection& operator=(const connection&) = delete;

      /// Construct a connection with the given socket.
      explicit connection(asio::io_service& io_service);

      /// Start the first asynchronous operation for the connection.
      void start();

      /// Stop all asynchronous operations associated with the connection.
      void stop();

      asio::ip::tcp::socket& getSocket();
    private:
      /// Perform an asynchronous read operation.
      void do_read();

      /// Perform an asynchronous write operation.
      void do_write();

      /// Socket for the connection.
      asio::ip::tcp::socket socket_;
      /// Buffer for incoming data.
      char buffer_[8192];


    };

    typedef std::shared_ptr<connection> connection_ptr;

  } // namespace server
} // namespace http

#endif // HTTP_CONNECTION_HPP