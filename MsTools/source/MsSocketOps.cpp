#include <MsSocketOps.h>
#include <MsBase.h>
#include <matchvs_log.h>




namespace matchvs
{
	Int32 socketInit()
	{
#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
		static bool bLoad = false;
		if (!bLoad)
		{
			WSADATA wsaData;

			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			{
				//LOGE("tag","init winsock failed！\n");
				return MS_FAIL;
			}

			if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
			{
				//LOGE("tag", "init winsock failed,please make sure Windows Sockets DLL 2.2 version\n");
				WSACleanup();
				return MS_FAIL;
			}
			bLoad = true;
		}
#endif
		return MS_OK;
	}

	uInt32 hostToNetwork32(uInt32 hostlong)
	{
		return htonl(hostlong);
	}

	uInt16 hostToNetwork16(uInt16 hostshort)
	{
		return htons(hostshort);
	}

	uInt32 networkToHost32(uInt32 netlong)
	{
		return ntohl(netlong);
	}

	uInt16 networkToHost16(uInt16 netshort)
	{
		return ntohs(netshort);
	}

	void fromHostPort(const Int8* ip, uInt16 port, struct sockaddr_in* addr)
	{
		addr->sin_family = AF_INET;
		addr->sin_port = hostToNetwork16(port);
		if (inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
		{
			//LOGE("tag", "sockets::fromHostPort\n");
		}
	}

	void toHostPort(Int8* buf, uInt32 size, uInt32 &port, struct sockaddr_in& addr)
	{
		char host[22] = "INVALID";
		inet_ntop(AF_INET, &addr.sin_addr, host, sizeof(host));
		port = networkToHost16(addr.sin_port);
		snprintf(buf, size, "%s", host);
	}

	void setNonBlock(uInt32 fd)
	{
#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
		uInt64 flag = 1;
		ioctlsocket(fd, FIONBIO, (unsigned long *)&flag);
#else
		uInt32 flag = fcntl(fd, F_GETFL, 0);
		fcntl(fd, F_SETFL, flag | O_NONBLOCK);
#endif
	}

	Int32 getSocketError(Int32 sockfd)
	{
		Int32 optval;
		socklen_t optlen = sizeof(optval);

		if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (Int8 *)(&optval), &optlen) < 0)
		{
			return errno;
		}
		else
		{
			return optval;
		}
	}
	void closeSocket(uInt32 sockfd)
	{
#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
		closesocket(sockfd);
#else
		close(sockfd);
#endif
	}
	void setBlock(uInt32 socket)
	{
#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
		uInt64 ul = 0;
		ioctlsocket(socket, FIONBIO, (unsigned long *)&ul);//设置成阻塞模式。  

#else
		fcntl(socket, F_SETFL, fcntl(socket, F_GETFL) & ~O_NONBLOCK);	
#endif
	}

	Int32 getIpByDomain(std::string &domain, std::string &ip)
	{
		struct addrinfo *answer, hint, *curr;
		char str[32];
		memset(&hint, 0, sizeof(hint));
		hint.ai_family = AF_INET;
		hint.ai_socktype = SOCK_STREAM;
		int ret = getaddrinfo(domain.c_str(), NULL, &hint, &answer);
		if (ret != 0) {
			return -1;
		}

		for (curr = answer; curr != NULL; curr = curr->ai_next) {
			if (((struct sockaddr_in *)(curr->ai_addr))->sin_addr.s_addr == 0) {
				continue;
			}
			inet_ntop(AF_INET,
				&(((struct sockaddr_in *)(curr->ai_addr))->sin_addr),
				str, 16);
			ip = str;
			freeaddrinfo(answer);
			return 0;
		}
		freeaddrinfo(answer);
		return -1;
	}
};