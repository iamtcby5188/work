#ifndef _HTTPCLIENT_H_
#define _HTTPCLIENT_H_
#include <string>
#include "logger.h"
#include <boost/asio.hpp>
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
using boost::asio::ip::tcp;
class HttpClient
{
public:
	HttpClient();
	HttpClient(const std::string&ip, const std::string&port);
	HttpClient(const std::string&ip, const std::string&port, const std::string&path,const std::string&content);
	~HttpClient(void);
public:
	void setIp(const std::string& ip){ip_ = ip;};
	void setPort(const std::string& port){port_ = port;};
	void setUrl(const std::string& path){path_ = path;};
	void setContent(const std::string& content){content_ = content;};
	void setTimeOut(const int& time_out){time_out_ = time_out;};
	const int& getTimeOut() const{return time_out_;};
	const std::string& getUrl() const{return path_;};
	const std::string& getIp() const{return ip_;};
	const std::string& getPort() const{return port_;};
	const std::string& getContent() const{return content_;};
	const std::string& getResponse() const{return body_;};
	void clear(){path_ = ""; content_ = "";};
	int post();

private:
	std::string getValidJson(const std::string& response);
	void wait_timeout(tcp::socket *socket);
	void incrementAliveThread();
	void decrementAliveThread();
	void interruptThread();

private:
	int time_out_;
	std::string ip_;
	std::string port_;
	std::string path_;
	std::string content_;

	std::string body_;
	boost::mutex mu_; 
	boost::condition_variable_any cond_; 

	int alive_thread_count;

};
#endif // _HTTPCLIENT_H_
