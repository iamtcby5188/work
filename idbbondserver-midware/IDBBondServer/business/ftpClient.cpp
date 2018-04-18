#include<time.h>
#include "ftpClient.h"
#include "logger.h"

typedef boost::shared_ptr<boost::thread> thread_ptr;
static boost::regex expression("^([0-9]+)(\\-| |$)(.*)$"); 

void ftp_client::WriteLog(char *strMsg){
	LOGGER_DEBUG(strMsg);
	/*time_t  now;
	struct tm *timenow;
    time(&now);
	timenow = localtime(&now);

	FILE *fp = fopen(fileName, "a+");
	fseek(fp,0,SEEK_END);
	char info[300] = {0};

	sprintf(info,"[%d:%d:%d] %s",timenow->tm_hour,timenow->tm_min,timenow->tm_sec,strMsg);
	fwrite(info,1,strlen(info),fp);
	fwrite("\r\n",1,strlen("\r\n"),fp);
	fclose(fp);*/
}

//构造函数
ftp_client::ftp_client():cmd_socket_(io_service_),data_socket_(io_service_)
{
	b_Logined = false;
	b_Connected = false;
	b_ExitNet = false;

	b_cmd_connect = false;
	b_data_connect = false;

	host_ = "";
	port_ = "";
	user_ = "";
	passwd_ = "";
	command_ = "";

	memset(fileName,0,40);
	sprintf(fileName,"ZYJ_%x.txt",boost::this_thread::get_id());

	WriteLog("construct finish!\n");
}   

//析构函数
ftp_client::~ftp_client()  
{
	WriteLog("[~ftp_client]data_socket_ destroy start!\n");

	if (data_socket_.is_open()){ 
		try{ 
			WriteLog("[~ftp_client]data_socket_ shutdown!\n");
			data_socket_.shutdown(boost::asio::socket_base::shutdown_both);
			WriteLog("[~ftp_client]data_socket_ close!\n");
			data_socket_.close();
		} catch (std::exception& e) {
			WriteLog("[~ftp_client]data_socket_ exception[FAIL]!\n");
			WriteLog((char *)(e.what()));
		}    
	}

	WriteLog("[~ftp_client]cmd_socket_ destroy start!\n");

	if (cmd_socket_.is_open()){
		try{ 
			WriteLog("[~ftp_client]cmd_socket_ shutdown!\n");
			cmd_socket_.shutdown(boost::asio::socket_base::shutdown_both);
			WriteLog("[~ftp_client]cmd_socket_ close!\n");
			cmd_socket_.close();  
		} catch (std::exception& e) {
			WriteLog("[~ftp_client]cmd_socket_ exception[FAIL]!\n");
			WriteLog((char *)(e.what()));
		}    	
	} 
	WriteLog("[~ftp_client]end!\n");
}
 
bool ftp_client::connect(std::string host,  std::string port, std::string user, std::string passwd)  
{  
	if (b_Connected){
		return true;  
	} 

	if (host.empty() || port.empty() || user.empty()){ 
		return false;  
	}  

	user_ = user; passwd_ = passwd; host_ = host; port_ = port;

	for(size_t i = 0; i < strlen(port_.c_str());++i)
	{
		if(i>10){
			return false;
		}
		if(port_.c_str()[i] > '9' || port_.c_str()[i] < '0')
		{
			return false;
		}
	}

	char info[200] = {0};
	sprintf(info,"[connect]ip=%s,port=%s,usr=%s\n",host.c_str(),port.c_str(),user.c_str());
	WriteLog(info);

	ip::tcp::resolver resolver(io_service_);  
	ip::tcp::resolver::query query(host_, port_);  
	ip::tcp::resolver::iterator it = resolver.resolve(query);
	ip::tcp::resolver::iterator end;
	boost::system::error_code  SyncError;
	if(it != end){
		//同步方式
		cmd_socket_.connect(*it, SyncError);
        LOGGER_ERROR("cmd_socket_.connect(): " << SyncError.value() << "\n" << SyncError.message());
	}else{
		WriteLog("[connect] ip::tcp::resolver::iterator [FAIL]\n");
	}

	if(SyncError){
		WriteLog("[connect]connect failed![FAIL]\n");
		char info[1000] = {0};
		sprintf(info, "[connect]cmd_socket_.connect(): SyncError.value: %d,SyncError.message:%s\n",SyncError.value(),SyncError.message().c_str());
		WriteLog(info);
		return false;
	}
	
	b_Connected = true;
	WriteLog("[connect]connect success\n");

	command_ = "\r\n";
	WriteLog("[connect](***while send command***)\n");
	while(true){
		if(b_ExitNet || b_Logined){
			break;
		}
		std::string strResult = "";
		SendCommand(command_,strResult);
		ParserResponse(strResult);
	}
	WriteLog("[connect](***while send over***)\n");
    return true;
}

bool ftp_client::connected()
{
	return b_Connected;
}

bool ftp_client::logined()
{
	return b_Logined;
}

void ftp_client::SetFilelistEmpty()
{
	while(!file_list.empty()){
		file_list.pop_front();
	}
}

//async upload file
void ftp_client::UploadFile(std::string filename)  
{
	if(!b_Logined){
		WriteLog("[UploadFile] Have not Logined!\n");
		return ;
	}
	char info[200] = {0};
	sprintf(info,"[UploadFile] filename: %s\n",(char *)filename.c_str());
	WriteLog(info);

	file_list.push_back(std::make_pair(true, filename));

	command_ ="PASV \r\n";
	while(1){
		if(b_ExitNet){
			break;
		}

		std::string strResult = "";
		SendCommand(command_,strResult);
		ParserResponse(strResult);
	}  
} 

//从本地文件读数据，通过data_socket_发送出去
bool ftp_client::SendFileData(std::string filename)  
{
	char buffer[1448]; 
	std::ifstream ifs(filename.c_str(), std::ifstream::binary);
	WriteLog("[SendFileData] (****while will be over?****)\n");

	while (1){  
		ifs.read(buffer, 1448);  
		data_socket_.send(boost::asio::buffer(buffer,ifs.gcount()));  
		if (ifs.gcount() < 1448){
			break;  
		}  
	}  
	WriteLog("[SendFileData] while over!\n");

	ifs.close(); 

	if(data_socket_.is_open()){
		try{
			WriteLog("[SendFileData]data_socket_ shutdown start!\n");
			data_socket_.shutdown(boost::asio::socket_base::shutdown_both);
			WriteLog("[SendFileData]data_socket_ close start!\n");
			data_socket_.close();
		}catch (std::exception& e){
			WriteLog((char *)(e.what()));
		}
	}

	return true;  
}  

//发送FTP命令
int ftp_client::SendCommand(std::string strCommand,std::string& strResult)  
{
	if (!cmd_socket_.is_open()){
		char logInfo[200] = {0};
		sprintf(logInfo,"cmd_socket_ is not open[FAIL]");
		WriteLog(logInfo);
		return 0;  
	}

	boost::asio::streambuf request;
	boost::asio::streambuf response;
	std::string rspStrLine;
	std::ostream request_stream(&request);
	std::istream response_stream(&response);

	if(strCommand == "UpLoadOver" || strCommand == "\r\n"){
		//接收回应
		try{
			boost::asio::read_until(cmd_socket_, response, "\r\n");
		}catch(std::exception& e){
			b_ExitNet = true;
			WriteLog((char *)(e.what()));
		}
		

		char info[200] = {0};
		sprintf(info,"[1]response length = %d\n",response.size());
		WriteLog(info);
		//获取字符串
		std::getline(response_stream, strResult);

		return strResult.length();
	}

    LOGGER_DEBUG("command is :" << strCommand);

	char logInfo[200] = {0};
	sprintf(logInfo,"[SendCommand]command is :%s",strCommand.c_str());
	WriteLog(logInfo);

	request_stream <<strCommand;

	//发送命令
	boost::asio::write(cmd_socket_, request);

	//接收回应
	try{
		boost::asio::read_until(cmd_socket_, response, "\r\n");
	}catch(std::exception& e){
		b_ExitNet = true;
		WriteLog((char *)(e.what()));
	}

	char info[200] = {0};
	sprintf(info,"[2]response length = %d\n",response.size());
	WriteLog(info);

	//获取字符串
	std::getline(response_stream, strResult);

	return strResult.length();

} 

void ftp_client::ParserResponse(std::string strResult)  
{  
	boost::cmatch what;
	if(boost::regex_match(strResult.c_str(), what, expression)){  
		HandleResponse(::atoi(what[1].first), std::string(what[3].first, what[3].second));  
	} 
}


//初始化Data_socket_
void ftp_client::ParserDataConInfo(std::string pasv)  
{
	WriteLog("parser_pasv function start!\n");
	if ((pasv.find('(') == std::string::npos) && (pasv.find(')') == std::string::npos)){ 
		WriteLog("[ParserDataConInfo] Response String [FAIL]");
		return;  
	}  
	std::pair<std::string, std::string>porthost = ParseHostPort(pasv);

	char info[200] = {0};
	sprintf(info, "[ParserDataConInfo] Data connect:ip = %s,port = %s",porthost.first.c_str(),porthost.second.c_str());
	WriteLog(info);

	ip::tcp::resolver resolver(io_service_); 
	ip::tcp::resolver::query query(porthost.first, porthost.second);
	ip::tcp::resolver::iterator it = resolver.resolve(query);  
	ip::tcp::endpoint endpoint = *it;  

	if (data_socket_.is_open()){  
		try{
			WriteLog("[ParserDataConInfo]data_socket_ shutdown!\n");
			data_socket_.shutdown(boost::asio::socket_base::shutdown_both);
			WriteLog("[ParserDataConInfo]data_socket_ close!\n");
			data_socket_.close();
		} catch (std::exception& e) {    
			WriteLog((char *)(e.what()));
		}    
	}

	boost::system::error_code  SyncError;
	data_socket_.connect(endpoint,SyncError); 

	if(SyncError){
		WriteLog("[ParserDataConInfo] Data socket connect failed[FAIL]!\n");
	}else{
		WriteLog("[ParserDataConInfo] Data socket connect success!\n");
	}	 
}


//解析出ip和端口
std::pair<std::string, std::string> ftp_client::ParseHostPort(std::string s)  
{  
	size_t paramspos = s.find('(');  
	std::string params = s.substr(paramspos + 1);  
	size_t ip1pos = params.find(',');  
	std::string ip1 = params.substr(0, ip1pos);  
	size_t ip2pos = params.find(',', ip1pos + 1);  
	std::string ip2 = params.substr(ip1pos + 1, ip2pos - ip1pos - 1);  
	size_t ip3pos = params.find(',', ip2pos + 1);  
	std::string ip3 = params.substr(ip2pos + 1, ip3pos - ip2pos - 1);  
	size_t ip4pos = params.find(',', ip3pos + 1);  
	std::string ip4 = params.substr(ip3pos + 1, ip4pos - ip3pos - 1);  
	size_t port1pos = params.find(',', ip4pos + 1);  
	std::string port1 = params.substr(ip4pos + 1, port1pos - ip4pos - 1);  
	size_t port2pos = params.find(')', port1pos + 1);  
	std::string port2 = params.substr(port1pos + 1, port2pos - port1pos - 1);  

	std::pair<std::string, std::string> hostport;  
	hostport.first = ip1 + "." + ip2 + "." + ip3 + "." + ip4;  
	int portval = atoi(port1.c_str()) * 256 + atoi(port2.c_str());  
	hostport.second = boost::lexical_cast<std::string>(portval);  
	return hostport;  
}

//处理收到的Cmd的命令
void ftp_client::HandleResponse(size_t resp_code, std::string resp_arg)  
{  
	std::string filename;

	char info[200] = {0};
	sprintf(info,"[HandleResponse]resp_code = %d, resp = %s\n",resp_code,resp_arg.c_str());
	WriteLog(info);

	switch (resp_code){  
	case 220:                                            //welcome
		{
			WriteLog("[RECV] welcome\n");
			command_= "User " + user_ +"\r\n";  
			break;
		}
	case 331:                                            // enter password  
		{
			WriteLog("[RECV] enter password\n");
			command_ = "Pass " + passwd_  +"\r\n";
			break;
		}
	case 230:                                            //loging success
		{
			WriteLog("[RECV] loging success,start send file code\n");
			b_Logined = true;
			break;
		}
	case 150:                                            //ftp server is reday for receive/trans data, client can  send/receive data     连续接收两个
		{
			WriteLog("[RECV] ftp server is reday for receive/trans data, client can  send/receive data \n");
			if (!file_list.empty()){ 
				if(true == file_list.front().first){
					WriteLog("[RECV] uploadfile start!\n");
					if ((SendFileData(file_list.front().second))){ 
                        LOGGER_DEBUG("file " << file_list.front().second << "is upload to server");
						WriteLog("[RECV] uploadfile success!\n");
						file_list.pop_front();  
					} 
				}
			}
			command_ = "UpLoadOver";
			break;
		}
	case 226:                                           // Transfer complete.
		{
			WriteLog("[RECV] Transfer complete\n"); 
			command_ = "QUIT\r\n";
			break;
		}
	case 227:                                          //Entering passive Mode
		{
			WriteLog("[RECV] Entering passive Mode\n");
			ParserDataConInfo(resp_arg);

			if (file_list.empty()){
				command_ = "QUIT\r\n";
				break;  
			}

			filename  = file_list.front().second;  
			if (file_list.front().first){
				filename = filename.substr(filename.find_last_of('/')+1);  
				command_ = "STOR " +filename + "\r\n";  
			}else{  
				command_ = "RETR " + filename + "\r\n";  
			}  
			break;
		}
	case 221:                                          //send quit command receive               
		{
			WriteLog("[RECV] send quit command receive\n");
			b_ExitNet = true;
			break;
		}

	case 530:                                         //Login or password incorrect!  
		{
			WriteLog("[RECV] Login or password incorrect! \n");
			command_ = "QUIT\r\n";
			break;	
		}  
	case 421:                                          // connect timeout  
		{
			WriteLog("[RECV] connect timeout! \n");
			SetFilelistEmpty();
			command_ = "QUIT\r\n";
			break;  
		}  
	case 550:                                         //File not found  
		{
			WriteLog("[RECV] File not found! \n");
			SetFilelistEmpty();
			command_ = "QUIT\r\n";
			break;  
		}  
	case 553:                                         //Could not create file. important file trans error
		{
			WriteLog("[RECV] Could not create file. important file trans error!\n");
			SetFilelistEmpty();
			command_ = "QUIT\r\n";
			break;
		}
	case 500:                                         //Permission denied
		{
			WriteLog("[RECV] Permission denied!\n");
			SetFilelistEmpty();
			command_ = "QUIT\r\n";
			break;
		}
	default:  
		{
			WriteLog("[RECV] other exception command[FAIL]!\n");
			SetFilelistEmpty();
			command_ = "QUIT\r\n";
			break;  
		}  
	}  
}


