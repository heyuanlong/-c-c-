#ifndef __MS_HTTP_HTTPREQUEST_H__
#define __MS_HTTP_HTTPREQUEST_H__

#include <MsBase.h>
#include <MsCustomError.h>
#include <MsHttpCommon.h>
#include <MsHttpResponse.h>
#include <map>


namespace matchvs
{
	class MsHttpRequest
	{
	public:
		MsHttpRequest();
		virtual ~MsHttpRequest();
		void setHead(const std::string& key, const std::string& value);
		void setParam(const std::string& key, const std::string& value);
		void setVersion(const std::string& version);
		void setFunc(const std::string& func);
		void setUrl(const std::string& url);
		void setHost(const std::string& host);
		void setPort(const int& port);
		void setBody(std::string  body);
		int  getErrorCode();
	public:
		void requestGet(std::string &result);
		void requestPost(std::string &result);
		int  sendGet(MsHttpResponse &rsp);
		int  sendPost(MsHttpResponse &rsp);
	private:
		void makeHead(HTTP_REQ_TYPE& type, const std::string & tempUrl, std::string &strHead);
		void makeParams(std::string & strInPara);
		int  send(MsHttpResponse &rsp, HTTP_REQ_TYPE type);
	private:
		std::string							version_;
		std::string							url_;
		std::string							host_;
		std::string							body_;
		int									port_;
		std::string							func_;
		std::map<std::string, std::string>	headers_;
		std::map<std::string, std::string>	inParams_;
		int									error_;
	};
};


#endif