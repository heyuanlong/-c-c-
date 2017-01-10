#include <MsHttpRequest.h>
#include <MsHttpCommon.h>
#include <sstream>
#include <stdio.h>
#include <MsHttpNet.h>
#include <MsSocketOps.h>

namespace matchvs
{
	MsHttpRequest::MsHttpRequest() :  version_(HTTP_VERSION_1_1)
									, port_(80)
									, func_("/")
	{

		headers_[HTTP_HEAD_ACCEPT]				= "*/*";
		headers_[HTTP_HEAD_ACCEPT_LANGUAGE]		= "zh-CN";
		headers_[HTTP_HEAD_USER_AGENT]			= "C++ http client";
		//headers_[HTTP_HEAD_CONNECTION]			= "Keep-Alive";
		socketInit();

	}

	MsHttpRequest::~MsHttpRequest()
	{

	}
	int  MsHttpRequest::sendGet(MsHttpResponse &rsp)
	{
		return send(rsp, HTTP_GET);
	}
	int  MsHttpRequest::sendPost(MsHttpResponse &rsp)
	{
		return send(rsp, HTTP_POST);
	}
	void MsHttpRequest::setHead(const std::string& key, const std::string& value)
	{
		headers_[key] = value;
	}

	void MsHttpRequest::setParam(const std::string& key, const std::string& value)
	{
		inParams_[key] = value;
	}

	void MsHttpRequest::setVersion(const std::string& version)
	{
		version_ = version;
	}

	void MsHttpRequest::setFunc(const std::string& func)
	{
		func_ = func;
	}

	void MsHttpRequest::setUrl(const std::string& url)
	{
		url_ = url;
	}

	void MsHttpRequest::setHost(const std::string& host)
	{
		host_ = host;
	}

	void MsHttpRequest::setPort(const int& port)
	{
		port_ = port;
	}

	void MsHttpRequest::makeHead(HTTP_REQ_TYPE& type, const std::string & tempUrl, std::string &strHead)
	{
		std::string strHttpType;

		switch (type)
		{
		case HTTP_GET: strHttpType = HTTP_REQUEST_GET; break;
		case HTTP_POST: strHttpType = HTTP_REQUEST_POST; break;
		case HTTP_PUT: strHttpType = HTTP_REQUEST_PUT; break;
		case HTTP_DELETE: strHttpType = HTTP_REQUEST_DELETE; break;
		}
		std::ostringstream oss;
		oss << strHttpType << " " << tempUrl << " " << version_ << "\r\n";
		oss << HTTP_HEAD_HOST << ": " << host_;
		if (port_ != 80)
		{
			oss << ":" << port_;
		}
		oss << "\r\n";
		for (std::map<std::string, std::string>::iterator it = headers_.begin(); it != headers_.end(); ++it)
		{
			oss << it->first << ": " << it->second << "\r\n";
		}
		oss << "\r\n";
		strHead.append(oss.str());
	}

	void MsHttpRequest::makeParams(std::string & strInPara)
	{
		std::map<std::string, std::string>::iterator it;
		std::string strSecond;

		for (it = inParams_.begin(); it != inParams_.end(); ++it)
		{
			if (it != inParams_.begin())
			{
				strInPara.append("&");
			}
			strInPara.append(it->first).append("=");
			strSecond = it->second;
			strInPara.append(strSecond);
		}
	}

	void MsHttpRequest::requestGet(std::string &result)
	{
		result = "";
		std::string strParam;
		HTTP_REQ_TYPE type = HTTP_GET;
		std::string tempUrl = url_;

		tempUrl.append(func_);
		tempUrl.append("?");

		makeParams(strParam);
		tempUrl.append(strParam);

		makeHead(type, tempUrl,result);
	}

	void MsHttpRequest::requestPost(std::string &result)
	{
		result = "";
		const int BUFLEN = 10;
		char szLen[BUFLEN] = { 0 };
		std::string strParam;
		HTTP_REQ_TYPE type = HTTP_POST;
		std::string tempUrl = url_;
		tempUrl.append(func_);
		makeParams(strParam);

		
		#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
			snprintf(szLen, BUFLEN, "%d", body_.size());
		#else
			sprintf(szLen, "%d", body_.size());
		#endif
		setHead(HTTP_HEAD_CONTENT_LENGTH, szLen);

		makeHead(type, tempUrl, result);

		result.append(body_);
	}
	void MsHttpRequest::setBody(std::string  body)
	{
		body_ = body;
	}
	int  MsHttpRequest::getErrorCode()
	{
		return error_;
	}
	int  MsHttpRequest::send(MsHttpResponse &rsp, HTTP_REQ_TYPE type)
	{
		rsp.clear();
		int ret;
		const int bufferMax = 50240 ;
		static char buffer[bufferMax];
		memset(buffer, 0, bufferMax);

		std::string ip;
		ret = getIpByDomain(host_, ip);						//应该增加 domain  -》 ip的缓存
		if (ret < 0) {
			error_ = MS_HTTP_GET_IP_FAIL;
			return MS_FAIL;
		}
		int fd = MsHttpNet::connectTcpByIp(ip);
		if (fd < 0) {
			error_ = MS_HTTP_CONNECT_FAIL;
			return MS_FAIL;
		}
		std::string packet;

		switch (type)
		{
		case HTTP_GET:requestGet(packet); break;
		case HTTP_POST: requestPost(packet); break;
		default:
			error_ = MS_HTTP_ERROR_TYPE;
			return -1;
		}
		//printf("~~~~~~~~%s~~~~~~~~~~~~", packet.c_str());
		ret = MsHttpNet::sendData(fd, packet.c_str(), packet.size());
		if (ret < 0) {
			closeSocket(fd);
			error_ = MS_HTTP_SEND_FAIL;
			return MS_FAIL;
		}

		ret = MsHttpNet::recvData(fd, buffer, bufferMax);
		if (ret < 0) {
			closeSocket(fd);
			error_ = MS_HTTP_RECV_FAIL;
			return MS_FAIL;
		}
		//printf("data:%s---------ret:%d\n", buffer,ret);
		rsp.appendResponse(buffer, ret);
		while ( !(rsp.checkFull(false)) )
		{
			memset(buffer, 0, bufferMax);
			ret = MsHttpNet::recvData(fd, buffer, bufferMax);
			if (ret <= 0) {
				//perror("-----------ret<=0----------");
				closeSocket(fd);
				error_ = MS_HTTP_RECV_FAIL;
				return MS_FAIL;
			}
			buffer[ret] = '\0';
			//printf(">>>>>>>>>>>>>>>>>>>>>%s--------------ret:%d \n", buffer, ret);
			rsp.appendResponse(buffer,ret);
		}
		closeSocket(fd);
		return MS_OK;
	}
};

