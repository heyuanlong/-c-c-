#include <MsSysUtil.h>
#include <string.h>
#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
#include <Winsock2.h>
#include <Windows.h>
#include <time.h>
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#endif//
#include <stdio.h>
#include <stdlib.h>
#if defined(__linux__) || defined(__linux) || defined(linux)
#include <sys/syscall.h>
#endif
namespace matchvs
{
	Int8* getErrStr(Int32 errorNo)
	{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined (__WIN32__)
		static char lpMsgBuf[128];
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			(LPTSTR)&lpMsgBuf,
			sizeof(lpMsgBuf), NULL);

		return lpMsgBuf;
#else
		return strerror(errorNo);
#endif
	}

	Int8* getErrStr()
	{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined (__WIN32__)
		static char lpMsgBuf[128];
		FormatMessage(
			/*FORMAT_MESSAGE_ALLOCATE_BUFFER | */FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			(LPTSTR)&lpMsgBuf,
			sizeof(lpMsgBuf), NULL);

		return lpMsgBuf;
#else
		return strerror(errno);
#endif
	}

	Int32 getErrno()
	{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined (__WIN32__)
		return GetLastError();
#else
		return errno;
#endif//
	}


	Int32 getWorkCwd(std::string &dir)
	{
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined (__WIN32__)
		char buffer[1024];
		_getcwd(buffer, 1023);
		dir = buffer;
		return MS_OK;
#else
		char buffer[1024];
		getcwd(buffer, 1023);

		dir = buffer;
		return MS_OK;
#endif//
	}

};