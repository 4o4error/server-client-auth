#include <cstdlib>
#include <iostream>
#include <asio.hpp>
#include <asio/ssl.hpp>
#include <functional>
#include <array>
#include <thread>
#include <memory>
#include <set>
#include <atomic>

#include "DbConnection.h"
#include "util\config\ConfigReader.h"
#include "util\config\ConfigClass.h"

//#include "vld.h"


typedef asio::ssl::stream< asio::ip::tcp::socket> ssl_socket;

 
 class session : public std::enable_shared_from_this<session>
 {
   session(const session&) = delete;
   session& operator=(const session&) = delete;
  std::string license ="";
public:
  session(asio::io_service& io_service,
    asio::ssl::context& context)
    : socket_(io_service, context)
  {
    
    checkConection = std::thread([this](){
      bool b= true;
      while (b){
       recentlyConnected = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        if (!recentlyConnected){
          std::cout << "shuting down " << std::endl;
          shutdownSession = true;
          b = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
      }
    });
    
    
 
  }

  ssl_socket::lowest_layer_type& socket()
  {
    return socket_.lowest_layer();
  }
  //parse request and extract http message body 
  void parseRequest(std::string& msg){
    //size_t found = msg.find("\r\n\r\n");
    //if (found != std::string::npos){
    //  msg = msg.substr(msg.find("\r\n\r\n") + 4, std::string::npos);
    //}
  }
  //handle ssl handshake
  void handle_handshake(){
   
    socket_.async_handshake(asio::ssl::stream_base::server,
      [this](std::error_code ec){
      if (!ec){
        handle_read();
      }
      });
  }
  //handle read 
  void handle_read(){

    socket_.async_read_some(asio::buffer(data_,max_length), [this](std::error_code ec, size_t bytesTransfered){
      if (!ec){
        
        if (shutdownSession){
          return;
        }
        
        std::cout << "reading in thread : " << std::this_thread::get_id() << std::endl;
        //recentlyConnected = true;
        std::stringstream ss;
        ss.write(data_, bytesTransfered);
        
        std::string msg = ss.str();
        parseRequest(msg);
        std::cout <<"message is "<< msg << std::endl;    
        handle_write();
      }
    });
  }  

  //handle write
  void handle_write(){
    
    const std::string head = " HTTP / 1.1 200 OK\r\n \
                           Content - Type: text/html; charset = utf - 8\r\n \
                           Content - Length:";
    std::stringstream ss;
    std::string message = "<html><head></head><body> hello</body></html>";
    ss << head << message.length()<<"\r\n"<< "\r\n\r\n" << message;
    
    if (license.compare("") == 0){
      dbconn::DbConnection db;
      license = db.generateLicense();
    }

    socket_.async_write_some(asio::buffer(license), [this](std::error_code ec, std::size_t){
      if (!ec){
        
        if (shutdownSession){
         // asio::error_code ignored_ec;
          //socket_.lowest_layer().shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
          return;
        }
        else{
          handle_read();
        }
      
      }
    });
  }
  ~session(){
    std::cout << "destroying session" << std::endl;
    asio::error_code ignored_ec;
    socket_.lowest_layer().shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }
private:
  ssl_socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  
  std::atomic<bool> recentlyConnected = false;
  std::atomic<bool> shutdownSession = false;
  std::atomic<bool> firstRead = true;
  std::thread checkConection;
  
};
typedef std::shared_ptr<session> session_ptr;

class server
{
public:
  server( asio::io_service& io_service, unsigned short port)
    : io_service_(io_service),
    acceptor_(io_service,
     asio::ip::tcp::endpoint( asio::ip::tcp::v4(), port)),
    context_(io_service,  asio::ssl::context::sslv23)
  {
    context_.set_options(
       asio::ssl::context::default_workarounds
      |  asio::ssl::context::no_sslv2
      |  asio::ssl::context::single_dh_use);
    context_.set_password_callback( std::bind(&server::get_password, this));
    context_.use_certificate_chain_file("cacert.pem");
    context_.use_private_key_file("private.pem",  asio::ssl::context::pem);
    context_.use_tmp_dh_file("dh512.pem");
    handle_accept();
  }
  // create and join threads 
  void start(){
    for (unsigned i = 0; i <1; ++i){
      threadPool.push_back(std::make_shared<std::thread>([&](){io_service_.run(); }));
    }
    std::for_each(threadPool.begin(), threadPool.end(), [](std::shared_ptr<std::thread> &t)
    {
      t->join();
    });
   
  }
 
  // return cert password 
  std::string get_password() const
  {
    return "test";
  }

  //handle new connection 
   void handle_accept()
  {
     bool repeat = true;

    try{
      session* new_session = new session(io_service_, context_);
      acceptor_.async_accept(new_session->socket(), [this,new_session](std::error_code ec){
        if (!acceptor_.is_open())
        {
          return;
        }
        if (!ec)
        {
          try{
       
           new_session->handle_handshake();
           
          }
          catch (std::exception& e){
            std::cout << e.what();
          }
        }
        else{
          std::cout << ec.message() << std::endl;
        }
        handle_accept();
      });
    
    }
    catch (std::exception& e){
      std::cerr << e.what() << std::endl;
    }
  
  }

private:
   //session pointer
   std::vector<session_ptr> new_sessions;
   asio::io_service& io_service_;
   asio::ip::tcp::acceptor acceptor_;
   asio::ssl::context context_;
   std::vector<std::shared_ptr<std::thread>> threadPool;
};

int main(int argc, char* argv[])
{
  try
  {
    config::ConfigReader cr;
    config::ConfigClass cc =cr.readIniFile<config::ConfigClass>("test.conf");
    asio::io_service io_service;
    server s(io_service, std::atoi(cc.getPort().c_str()));
    s.start();

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}