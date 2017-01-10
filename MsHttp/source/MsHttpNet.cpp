#include <MsHttpNet.h>
#include <MsSocketOps.h>

namespace matchvs
{

	MsHttpNet::MsHttpNet()
	{
	}

	MsHttpNet::~MsHttpNet()
	{
	}

	int MsHttpNet::connectTcpByIp(std::string &ip) //超时connect
	{
		socketInit();
		int tmpSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (tmpSocket < 0) {
			return -1;
		}
		struct sockaddr_in localAddr;
		memset(&localAddr, 0, sizeof(struct sockaddr_in));
		localAddr.sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), (void*)&localAddr.sin_addr.s_addr);
		localAddr.sin_port = hostToNetwork16(80);
		setNonBlock(tmpSocket);

		if (connect(tmpSocket, (struct sockaddr*)&localAddr, sizeof(struct sockaddr)) < 0) {
#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
			if (GetLastError() == WSAEWOULDBLOCK) {
#else
			if (errno == EINPROGRESS || errno == EWOULDBLOCK) {
#endif
			
				fd_set tmpSet;
				FD_ZERO(&tmpSet);
				FD_SET(tmpSocket, &tmpSet);
				struct timeval tv;
				tv.tv_sec = 1;
				tv.tv_usec = 0;
				if (select((tmpSocket + 1), NULL, &tmpSet, NULL, &tv) > 0 ){
					int so_error;
					socklen_t len = sizeof so_error;
					getsockopt(tmpSocket, SOL_SOCKET, SO_ERROR, (char*)&so_error, &len);
					if (so_error != 0) {
						closeSocket(tmpSocket);
						return -1;	//someError
					}
					else {
						//success
					}
				}
				else {
					closeSocket(tmpSocket);
					return -1; //timeout
				}
			}
			else {
				//success
			}
		}
		//success
		setBlock(tmpSocket);

#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
		int timeout = 500; //0.5秒
#else
		struct timeval timeout = { 0,500000 };//0.5秒
#endif
		if (setsockopt(tmpSocket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout)) < 0) {
			closeSocket(tmpSocket);
			return -1;

		}
		if (setsockopt(tmpSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) < 0) {
			closeSocket(tmpSocket);
			return -1;

		}
		return tmpSocket;
	}

	int MsHttpNet::sendData(Int32 fd, const char *buffer, int len)
	{
		int send_len = 0;
		int have_len = 0;
		while ((len - have_len) > 0) {
			send_len = send(fd, buffer + have_len, len - have_len, 0);  //已经设置了超时
			if (send_len < 0) {
				return -1;
			}
			have_len += send_len;
		}
		return have_len;
	}
	int MsHttpNet::recvData(Int32 fd, char *buffer, int len)
	{
		int recv_len = 0;
		recv_len = recv(fd, buffer, len, 0);  //已经设置了超时
		if (recv_len < 0) {
			return -1;
		}
		return recv_len;
	}


};

