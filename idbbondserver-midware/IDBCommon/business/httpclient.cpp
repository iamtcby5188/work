#include "httpclient.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp> 
#include <boost/bind.hpp>
#include <boost/function/function0.hpp>

HttpClient::HttpClient(){
	alive_thread_count = 0;
	time_out_ = 10;
}

HttpClient::HttpClient(const std::string&ip, const std::string&port):
ip_(ip),port_(port)
{
	alive_thread_count = 0;
	time_out_ = 10;
}

HttpClient::HttpClient(const std::string&ip, const std::string&port, const std::string&path,const std::string&content):
ip_(ip),port_(port),path_(path),content_(content)
{
	alive_thread_count = 0;
	time_out_ = 10;
}

HttpClient::~HttpClient(void)
{
}

void HttpClient::incrementAliveThread()
{
	boost::mutex::scoped_lock lock(mu_); 
	alive_thread_count++;	
	LOGGER_DEBUG("incrementAliveThread " << alive_thread_count);
}
void HttpClient::decrementAliveThread()
{
	boost::mutex::scoped_lock lock(mu_); 
	alive_thread_count--;	
	LOGGER_DEBUG("decrementAliveThread " << alive_thread_count);
}
void HttpClient::interruptThread()
{
	while(alive_thread_count)
	{
		LOGGER_DEBUG("Force the thread finish");
		boost::mutex::scoped_lock lock(mu_); 
		cond_.notify_all(); 
		boost::this_thread::sleep(boost::posix_time::milliseconds(5)); 
	}

}
void HttpClient::wait_timeout(tcp::socket *socket) 
{ 
	{
		boost::mutex::scoped_lock lock(mu_); 
		if (cond_.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(time_out_)))   
		{  
			LOGGER_DEBUG("recieved notification!");  
		} else {  
			LOGGER_ERROR("timeout, didn't recieve notification!");   
			socket->close();
		}
	}
	decrementAliveThread();
}

int HttpClient::post()
{
	try
	{
		int ret = 0;
		alive_thread_count = 0;
		// Get a list of endpoints corresponding to the server name.
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(ip_, port_);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		// Try each endpoint until we successfully establish a connection.
		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);

		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		//request_stream << "POST " << path_  << " HTTP/1.1\r\n";
		request_stream << "POST " << path_  << " HTTP/1.1\r\n";
		request_stream << "Host: " << ip_ << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n";			
		request_stream << "Content-Type: application/json; charset=utf-8 \r\n";
		request_stream << "Content-Length: " << content_.length() << "\r\n";
		request_stream  << "\r\n";
		request_stream  << content_;
		
		// Send the request.
		boost::function0<void> f = boost::bind(&HttpClient::wait_timeout,this, &socket); 
		incrementAliveThread();
	    boost::thread t(f); 
		boost::asio::write(socket, request);
		// Read the response status line. The response streambuf will automatically
		// grow to accommodate the entire line. The growth may be limited by passing
		// a maximum size to the streambuf constructor.
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");
		// Check that response is OK.
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			interruptThread();
			LOGGER_ERROR("Invalid response" );
			return 1;
		}

		if (status_code != 200)
		{
			interruptThread();
			LOGGER_ERROR("Response returned with status code " << status_code);
			return 2;
		}

		// Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(socket, response, "\r\n\r\n");
		// Process the response headers.
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
		{
			LOGGER_DEBUG(header);
		}

		// Write whatever content we already have to output.
		std::ostringstream ss;
		if (response.size() > 0)
		{
			ss << &response;
			body_ = ss.str();
		}

		// Read until EOF, writing data to output as we go.
		boost::system::error_code error;
		while (boost::asio::read(socket, response,
			boost::asio::transfer_at_least(1), error))
		{	
			ss << &response;
			body_ += ss.str();
		}

		{
			boost::mutex::scoped_lock lock(mu_); 
			cond_.notify_all();
		}
		LOGGER_DEBUG("original response body: " << body_);
		body_ = getValidJson(body_);
		LOGGER_DEBUG("valid response body: " << body_);

		if (error != boost::asio::error::eof)
			throw boost::system::system_error(error);
	}
	catch (std::exception& e)
	{
		interruptThread();
		LOGGER_ERROR("Exception: " << e.what());
		return 3;
	}
	interruptThread();
	LOGGER_INFO("post request succeed");
	return 0;
}

std::string HttpClient::getValidJson(const std::string & response)
{
	std::size_t start_pos = response.find_first_of("{");
	std::size_t end_pos = response.find_first_of("}");
	std::size_t len= end_pos - start_pos;
	if (len)
	{
		return response.substr(start_pos,len+1);
	}
	return response;
}