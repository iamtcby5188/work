#ifndef _FTP_CLIENT_H_
#define _FTP_CLIENT_H_
#include <boost/regex.hpp>  
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/format.hpp>
#include <boost/asio/write.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp> 

using namespace boost::asio;
#include <stdio.h>

#define SINGLETON

struct FTP_RULES {
    std::string goodsTypeId;
    int bidFloat;
    int ofrFloat;
    std::string goodsTypeCode;
};

struct FTP {
    std::string ftpId;
    std::string ftpIp;
    std::string ftpPort;
    std::string ftpUsername;
    std::string ftpPassword;
    int startTime;
    int endTime;
    int interval;
};

class ftp_client:public boost::noncopyable  
{  
public:  	
#ifdef SINGLETON  
	static ftp_client & getInstence()  
	{  
		static ftp_client ftp;  
		return ftp;  
	}  
#endif

public:
	ftp_client();  
	~ftp_client();  

	void WriteLog(char *strMsg);

	bool connect(std::string host,  std::string port, std::string user, std::string passwd);
	bool connected();
	bool logined();
	void UploadFile(std::string filename);  

protected:  
	int  SendCommand(std::string strCommand, std::string& strResult);
	bool SendFileData(std::string filename);

	void HandleResponse(size_t resp_code, std::string resp_arg);
	void ParserResponse(std::string strResult);
	std::pair<std::string, std::string> ParseHostPort(std::string s);
	void ParserDataConInfo(std::string pasv); 
	
	void SetFilelistEmpty(); 

private:  
	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::socket cmd_socket_;  
	boost::asio::ip::tcp::socket data_socket_;

	std::list<std::pair<bool, std::string>> file_list;  

	std::string host_;  
	std::string port_;  
	std::string user_;  
	std::string passwd_;
	std::string command_; 

	bool b_Logined;
	bool b_Connected;    
	bool b_ExitNet;

	bool b_cmd_connect;
	bool b_data_connect;

	char fileName[40];
};  
#endif // _FTP_CLIENT_H_
