#ifndef __MS_SOCKET_OPS_H__
#define __MS_SOCKET_OPS_H__
#include <MsBase.h>
#include <string>
#include <string.h>

#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <fcntl.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#endif//


namespace matchvs
{
	Int32 socketInit();
	uInt32 hostToNetwork32(uInt32 hostlong);
	uInt16 hostToNetwork16(uInt16 hostshort);
	uInt32 networkToHost32(uInt32 netlong);
	uInt16 networkToHost16(uInt16 netshort);
	void fromHostPort(const Int8* ip, uInt16 port, struct sockaddr_in* addr);
	void toHostPort(Int8* buf, uInt32 size, uInt32 &port, struct sockaddr_in& addr);
	void setNonBlock(uInt32 fd);
	Int32 getSocketError(Int32 sockfd);
	void closeSocket(uInt32 sockfd);
	void setBlock(uInt32 socket);
	Int32 getIpByDomain(std::string &domain, std::string &ip);
};

#endif