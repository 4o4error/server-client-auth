#pragma once;

#ifdef SSLAUTHCIENTDLL_EXPORTS
#define AUTHCLIENTDLL_API __declspec(dllexport) 
#else
#define AUTHCLIENTDLL_API __declspec(dllimport) 
#endif

#include <cstdlib>
#include <iostream>
#include <asio.hpp>
#include <asio/ssl.hpp>
#include <functional>
#include <memory>
#include <thread>
#include <chrono>
#include <future>
#include <fstream>

enum { max_length = 1024 };

class AUTHCLIENTDLL_API client
{
public:

	static client& Instance();
	
	void init();
	void set_host(const std::string& host);
	void set_port(const std::string& port);
	void set_certificate(const std::string& certificate);
	void setLostConnectionNotifier(const std::function<void(void)>& callback);
	void setLostConnectionNotifier(std::function<void(void)>&& callback);
	void initRemoteHostData(const std::string& host, const std::string& port, const std::string& certificate);
	std::string license();
	void request_license();
	void get_license();
	void destroy();
	~client();

private:

	//client(std::string host, std::string port, std::string certificate, std::function<void(void)>&& callback);
	client();
	client(const client &old) = delete; // disallow copy constructor
	
	const client &operator=(const client &old) = delete; //disallow assignment operator
	bool verify_certificate(bool preverified, asio::ssl::verify_context& ctx);
	void connect(asio::ip::tcp::resolver::iterator& endpoint_iterator);
	void handle_handshake();
	void handle_write();
	void handle_read();
	bool closeConnection(bool&& close_connection = false);

	std::mutex mtx;
	class impl;
	std::unique_ptr<impl> impl_ptr;
	
	
};


