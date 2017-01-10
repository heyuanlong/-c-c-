#ifndef __MS_HTTP__NET_H__
#define __MS_HTTP__NET_H__

#include <MsBase.h>
#include <string>
#include <string.h>

#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
#include <WS2tcpip.h>
#include <Winsock2.h>
#include <windows.h>
#else
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#endif//

namespace matchvs
{
	class MsHttpNet
	{
	public:
		MsHttpNet();
		~MsHttpNet();

	public:
		static int connectTcpByIp(std::string &ip);
		static int sendData(Int32 fd, const char *buffer, int len);
		static int recvData(Int32 fd, char *buffer,int len);

	private:

	};
	
};


#endif