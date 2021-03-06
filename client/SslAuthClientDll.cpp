#include "SslClientAuthDll.h"

namespace auth
{
	class client::impl
	{
		client::impl()
			: context_(asio::ssl::context::sslv23),
			socket_(io_service_, context_)
		{
			this->host_ = "";
			this->port_ = "";
			this->certificate_ = "";
			this->request_ = "";
			this->id_ = "0000";
			this->license_ = "Nothing";
			this->license_is_set_ = false;
			this->close_connection_ = false;
			this->background_thread = nullptr;
			this->future_ = this->promise_.get_future();
		}

		asio::io_service io_service_;
		asio::ssl::context context_;
		asio::ssl::stream<asio::ip::tcp::socket> socket_;
		std::string request_;
		char reply_[max_length];
		bool license_is_set_;
		bool close_connection_;
		std::string license_;
		std::string certificate_;
		std::string host_;
		std::string port_;
		std::string id_;
		std::unique_ptr<std::thread> background_thread;
		std::function<void(void)> callback = [](){};
		asio::ip::tcp::resolver::iterator endpoint_iterator;
		std::promise<std::string> promise_;
		std::future<std::string> future_;
		friend client;
	};

	client::client()
		: impl_ptr(new impl())
	{
		impl_ptr->socket_.set_verify_mode(asio::ssl::verify_peer);
		auto f = std::bind(&client::verify_certificate, this, std::placeholders::_1, std::placeholders::_2);
		impl_ptr->socket_.set_verify_callback(f);
	}

	client::~client()
	{

	}

	void client::destroy()
	{

		impl_ptr->close_connection_ = true;

		if (impl_ptr->background_thread->joinable())
		{
			impl_ptr->background_thread->join();
		}
	}

	client* client::Instance()
	{
	  client* instance = new client();
		return instance;
	}

	void client::initRemoteHostData(const std::string& host, const std::string& port, const std::string& certificate)
	{
		this->impl_ptr->host_ = host;
		this->impl_ptr->port_ = port;
		this->impl_ptr->certificate_ = certificate;
		impl_ptr->context_.load_verify_file(impl_ptr->certificate_);
		asio::ip::tcp::resolver resolver(impl_ptr->io_service_);
		asio::ip::tcp::resolver::query query(impl_ptr->host_, impl_ptr->port_);
		impl_ptr->endpoint_iterator = resolver.resolve(query);
	}

	void client::set_my_id(const std::string& id)
	{
		this->impl_ptr->id_= id;
	}

	void client::setLostConnectionNotifier(const std::function<void(void)>& callback)
	{
		impl_ptr->callback = callback;
	}

	void client::setLostConnectionNotifier(std::function<void(void)>&& callback)
	{
		impl_ptr->callback = std::move(callback);
	}

	std::string client::license()
	{
		if (!impl_ptr->license_is_set_)
		{
			std::string s = impl_ptr->future_.get();
			return s;

		}

		return impl_ptr->license_;
	};

	void client::request_license()
	{
		try
		{
			std::string result = "command=request_license id=";
			result += this->impl_ptr->id_;
			std::stringstream buff;
			buff << "POST HTTP/1.1\r\n";
			buff << "Host: localhost:4242\r\n";
			buff << "Content-Length: " << result.length() << "\r\n\r\n";
			buff << result;
			this->impl_ptr->request_ = buff.str();
			this->connect(impl_ptr->endpoint_iterator);
			impl_ptr->io_service_.run();
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
		}
	}

	void client::get_license()
	{
		impl_ptr->background_thread = std::make_unique<std::thread>(&client::request_license, this);
	}

	bool client::verify_certificate(bool preverified, asio::ssl::verify_context& ctx)
	{
		// The verify callback can be used to check whether the certificate that is
		// being presented is valid for the peer. For example, RFC 2818 describes
		// the steps involved in doing this for HTTPS. Consult the OpenSSL
		// documentation for more details. Note that the callback is called once
		// for each certificate in the certificate chain, starting from the root
		// certificate authority.

		// In this example we will simply print the certificate's subject name.
		char subject_name[256];
		X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
		X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
		std::cout << "Verifying " << subject_name << "\n";

		return preverified;
	}

	void client::connect(asio::ip::tcp::resolver::iterator& endpoint_iterator)
	{
		asio::async_connect(impl_ptr->socket_.lowest_layer(),
			endpoint_iterator,
			[this](const std::error_code& error, asio::ip::tcp::resolver::iterator iterator)
		{
			if (!error)
			{
				handle_handshake();
			}
			else
			{
				std::cout << "\nConnect failed: " << error.message() << "\n";
			}
		}
		);
	}

	void client::handle_handshake()
	{
		impl_ptr->socket_.async_handshake(asio::ssl::stream_base::client,
			[this](const std::error_code& error)
		{
			if (!error)
			{
				handle_write();
			}
			else
			{
				std::cout << "\nHandshake failed: " << error.message() << "\n";
			}
		}
		);
	}

	void client::handle_write()
	{

		if (this->impl_ptr->close_connection_)
		{
			return;
		}

		impl_ptr->socket_.async_write_some(asio::buffer(impl_ptr->request_),
			[this](std::error_code ec,
			size_t bytes_transferred)
		{
			if (!ec)
			{
				handle_read();
			}
			else
			{
				std::cout << "\nWrite failed: " << ec.message() << "\n";
				impl_ptr->license_ = "Nothing";
				impl_ptr->callback();
			}
		}
		);
	}

	void client::handle_read()
	{

		if (this->impl_ptr->close_connection_)
		{

			return;
		}

		impl_ptr->socket_.async_read_some(

			asio::buffer(impl_ptr->reply_, max_length),
			[this](std::error_code ec, size_t bytes_transferred)
		{
			if (!ec)
			{
				std::stringstream ss;
				ss.write(impl_ptr->reply_, bytes_transferred);
				std::string msg = ss.str();
				
				this->extract_license(msg);
				if (impl_ptr->license_.compare("Nothing") == 0)
				{
					impl_ptr->license_ = msg;
					impl_ptr->promise_.set_value(msg);
					impl_ptr->license_is_set_ = true;
				}
				else
				{
					try
					{
						if (impl_ptr->license_.compare(msg) != 0)
						{
							throw std::exception("Different license granted from server...");
						}
					}
					catch (std::exception& e)
					{
						std::cout << "\nAn exception occurred. " << e.what() << "\n";
					}
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				handle_write();
			}
			else
			{
				std::cout << "\nRead failed: " << ec.message() << "\n";
				impl_ptr->license_ = "Nothing";
				impl_ptr->callback();
			}
		}
		);

	}

	void client::extract_license(std::string& reply)
	{
		size_t found_body = reply.find("\r\n\r\n");
		if (found_body != std::string::npos)
		{
			reply = reply.substr(found_body + 4);
		}
	}
}

