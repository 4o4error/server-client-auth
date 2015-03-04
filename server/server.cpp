#include <cstdlib>
#include <iostream>
#include <memory>
#include <functional>
#include <utility>
#include <thread>
#include <asio.hpp>
#include <asio\ssl.hpp>
#include <chrono>
#include <mutex>
#include <set>
#include <vector>

const std::string dbName ="Database"; 

//sqllite db interface
#include "sqliteInterface\Singleton.h"

using asio::ip::tcp;

typedef asio::ssl::stream< asio::ip::tcp::socket> ssl_socket;
class session
  : public std::enable_shared_from_this<session>
{
public:
  session(asio::io_service& io_service, asio::ssl::context& context)
    : socket_(io_service, context)
  {
    Singleton::getInstance()->openDatabase(dbName);
    
  }
  bool readyToShutDown(){
    return shutdownSession;
  }
  void start() {
    std::cout << "shaking hands" << std::endl;
    auto self(shared_from_this());
    socket_.async_handshake(asio::ssl::stream_base::server, [this,self](std::error_code ec){
      if (!ec){
        do_work();
      }
      else{
        std::cout << ec.message() << std::endl;
      }
    });
  }
  ssl_socket::lowest_layer_type& socket()
  {
    return socket_.lowest_layer();
  }
  ~session(){
    std::cout << "destroying session " << std::endl;
    Singleton::getInstance()->resetLicence("licenses", license);
    std::error_code ec; 
    socket_.lowest_layer().shutdown(asio::ip::tcp::socket::shutdown_both, ec);
    
  }
private:
  void do_work(){
    readWriteThread = std::thread(&session::do_readwrite, this);
  }

  // parse incoming message for command
  // @msg = msg to parse
  // @s = string to parse for
  std::string  parseMessageFor(std::string msg, std::string s){
    //s.append("=");
    std::size_t found = msg.find(s);
    if (found != std::string::npos)
    {
      // posiotion found + command size + '='
      size_t separator = msg.find(" ",0);
      std::string result = msg.substr(found, separator-3);
     // result = msg.substr(result.find("=")+1, std::string::npos);
      result = result.substr(s.size()+1,std::string::npos);
      result = trim(result);
      return result;
      
    }
    else
    {
      return "";
    }
  }
  void processPost(std::string& msg){
    size_t foundMsgBody = msg.find("\r\n\r\n");
    if (foundMsgBody != std::string::npos)
    {
      msg = msg.substr(foundMsgBody, std::string::npos);
    }
    
  }

  void processGet(std::string msg){
  
  }

  void buildPostResponse(std::string status){
    std::string header = "HTTP / 1.1 "+status+"\r\nServer: auth-server\r\n Content - type: text / html \r\n\r\n";
    httpresponse = header;
    httpresponse.append(license);
  }
  inline std::string trim(const std::string& s) {
    if (s.length() == 0)
      return s;
    int b = s.find_first_not_of(" \t");
    int e = s.find_last_not_of(" \t");
    if (b == -1) // No non-spaces
      return "";
    return std::string(s, b, e - b + 1);
  }
  void do_readwrite(){
    auto lastConnectionTime = std::chrono::steady_clock::now();
    while (!shutdownSession){

      //get difference since last connection
      auto currentTime = std::chrono::steady_clock::now();
      std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastConnectionTime);
      auto diffInLong = diff.count();
      // check timeout period 
      if (diffInLong < std::chrono::milliseconds(5000).count()){
        //std::cout << "still under timeout period" << std::endl;
        if (wroteStuff){
          wroteStuff = false;
          std::error_code ec;
          size_t bytesTransfered = socket_.read_some(asio::buffer(data_, max_length), ec);
          if (bytesTransfered > 0)
          {
            lastConnectionTime = std::chrono::steady_clock::now();
            std::stringstream ss;
            ss.write(data_, bytesTransfered);
            std::string msg = ss.str();
            if (msg.substr(0, 4) == "POST"){
              processPost(msg);
              
              std::string command = parseMessageFor(msg, "command");
              std::string id = parseMessageFor(msg, "id");
              std::cout << "command is " << command << std::endl << "id is " << id << std::endl;
              if (command.compare("request_license") == 0){
                if (license.compare("") == 0)
                {
                  license = Singleton::getInstance()->getUnusedLicences(msg);
                }
              }
              buildPostResponse("200 OK");
            }
            else
            {
              processGet(msg);
              std::string header = "HTTP / 1.1 405 Method Not Allowed \r\n Content - type: text / html \r\n\r\n";
              httpresponse = header;
            }
          
            readStuff = true;
            //TODO Process stuff 
           
          }
        }
        if (readStuff)
        {
          readStuff = false;
          std::error_code ec;
          std::string header = "HTTP / 1.0 200 OK \r\n Content - type: text / html \r\n\r\n";
          
          std::size_t bytes_transfered = socket_.write_some(asio::buffer(license), ec);
          if (!ec){
            wroteStuff = true;
            std::cout << "wrote " << httpresponse << std::endl;
          }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      else{
        prepareToBeDestroyed();

      }
    }
  }
 void prepareToBeDestroyed(){
   readWriteThread.detach();
   shutdownSession = true;
  // removeThis(this);
 }
  void do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(
      asio::buffer(data_, max_length),
      [this, self](std::error_code ec, std::size_t length)
    {
      if (!ec) {
        std::stringstream ss;
        ss.write(data_, length);
        std::string msg = ss.str();
        std::cout << "message is " << msg << std::endl;
        do_write(length);
      }
    }
    );
  }

  void do_write(std::size_t length) {
    auto self(shared_from_this());
    asio::async_write(
      socket_,
      asio::buffer(data_, length),
      [this, self](std::error_code ec, std::size_t /*length*/)
    {
      if (!ec) {
        do_read();
      }
    }
    );
  }
  std::string httpresponse = "";
  std::string license = "";
  bool wroteStuff =true;
  bool readStuff = false;
  bool shutdownSession =false;
  std::thread readWriteThread;
  ssl_socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
};


//

class ConnectionManager{
    std::mutex mutex;
    std::set<std::shared_ptr<session>> sessions;
    std::thread checkIfWorkingThread;
  public:
    ConnectionManager(ConnectionManager&) = delete;
    ConnectionManager& operator=(ConnectionManager&) = delete;
    ConnectionManager(){
      sessions.clear(); 
      checkIfWorkingThread = std::thread(&ConnectionManager::checkIfWorking, this);
    }
    ~ConnectionManager(){
      checkIfWorkingThread.join();
    }
    void addSession(std::shared_ptr<session> session){
      std::lock_guard<std::mutex> lg(mutex);
      sessions.insert(session);
    }
       
    void checkIfWorking(){
      std::vector<std::shared_ptr<session>> clearingVec;
      while (true)
      {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (!sessions.empty())
        {
          for (std::set <std::shared_ptr<session>>::iterator it = sessions.begin(); it != sessions.end(); ++it)
          {
            if (it->get()->readyToShutDown())
            {
              std::lock_guard<std::mutex> lg(mutex);
              clearingVec.push_back(*it); 
            }
          }
          for each (auto var in clearingVec)
          {
            std::lock_guard<std::mutex> lg(mutex);
            sessions.erase((var));
          }
          clearingVec.clear();
        }
      }
    }
  };
  
  class ConnectionManagerFactory{
  public:
    static ConnectionManager* getInstance(){
      static ConnectionManager Instance;
      return &Instance;
    }
  
  };


class server {
public:
  server(asio::io_service& io_service, short port)
    : io_service_(io_service),
    acceptor_(io_service,
    asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
    socket_(io_service),
    context_(io_service, asio::ssl::context::sslv23)
  {
    context_.set_options(
      asio::ssl::context::default_workarounds
      | asio::ssl::context::no_sslv2
      | asio::ssl::context::single_dh_use);
    context_.set_password_callback(std::bind(&server::get_password, this));
    context_.use_certificate_chain_file("cacert.pem");
    context_.use_private_key_file("private.pem", asio::ssl::context::pem);
    context_.use_tmp_dh_file("dh512.pem");
    
    //create database; 
    Singleton::getInstance()->createTablesAndDatabase();
    
    Singleton::getInstance()->resetLicence("licenses", "alex");
    Singleton::getInstance()->resetLicence("licenses", "vlad");
    Singleton::getInstance()->resetLicence("licenses", "adi");
    Singleton::getInstance()->displayTable("licenses");
    do_accept();
  }

private:

  std::string get_password() const{
    return "test";
  }
  void do_accept() {
    std::shared_ptr<session> shared_session = std::make_shared<session>(io_service_, context_);
    acceptor_.async_accept(
      shared_session->socket(),
      [this, shared_session](std::error_code ec)
    {
      if (!ec) {
        shared_session->start();  // according to stack o. it should do the same thing
        ConnectionManagerFactory::getInstance()->addSession(shared_session);
      }

      do_accept();
    });
  }
 
 
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  asio::io_service& io_service_;
  asio::ssl::context context_;
};


int main(int argc, char* argv[]) {
  try {
   
    asio::io_service io_service;

    server s(io_service, 4242);

    io_service.run();

  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}