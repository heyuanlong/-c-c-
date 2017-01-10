
#include <MsHttpUtil.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <zlib.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#define stricmp strcasecmp
#else
#include <WinSock2.h>
#include <Ws2tcpip.h>
#define close(s) closesocket(s)
#define stricmp _stricmp
#endif


using namespace std;



MsHttpUtil::MsHttpUtil()
{

}

MsHttpUtil::~MsHttpUtil()
{

}

int MsHttpUtil::compressGzipInit(unsigned char* dst, int dstsize)
{
    m_strm = { 0 };
    m_strm.zalloc = Z_NULL;
    m_strm.zfree = Z_NULL;
    m_strm.opaque = Z_NULL;
    m_strm.avail_in = 0;
    m_strm.next_in = Z_NULL;
    int ret = deflateInit2(&m_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        return DEPRESS_ERROR;
    }

    m_strm.avail_out = dstsize;
    m_strm.next_out = (Bytef*)dst;

    return OK;
}

int MsHttpUtil::compressGzipEnd(int &size)
{
    size = m_strm.total_out;
    inflateEnd(&m_strm);
    return OK;
}

int MsHttpUtil::compressGzip(const char* src, int size)
{
    m_strm.avail_in = size;
    m_strm.next_in = (Bytef*)src;
    int ret = deflate(&m_strm, Z_NO_FLUSH);
    if (ret != Z_OK) {
        //WHATSLOG("gzip init error!");
    }

    return 0;
}






int MsHttpUtil::depressGzipInit(unsigned char* dst, int dstsize)
{
    m_strm = { 0 };
    m_strm.zalloc = Z_NULL;
    m_strm.zfree = Z_NULL;
    m_strm.opaque = Z_NULL;
    m_strm.avail_in = 0;
    m_strm.next_in = Z_NULL;
    int ret = inflateInit2(&m_strm, MAX_WBITS + 16);
    if (ret != Z_OK) {
        return DEPRESS_ERROR;
    }

    m_strm.avail_out = dstsize;
    m_strm.next_out = (Bytef*)dst;
    

    return OK;
}

int MsHttpUtil::depressGzipEnd(int &size)
{
    size = m_strm.total_out;
    inflateEnd(&m_strm);
    return OK;
}

int MsHttpUtil::depressGzip(const char* src, int size)
{
    m_strm.avail_in = size;
    m_strm.next_in = (Bytef*)src;
    int ret = inflate(&m_strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END) {
        //WHATSLOG("gzip depress error!");
    }

    return 0;
}

int  MsHttpUtil::checkUrlFormat(std::string &url)
{
    int len = url.length();
    if (len < 9) {
        return URL_FORMAT_WRONG;
    }

    std::string  bg = url.substr(0, 7);
    static const char* http = "http://";
    
    if (stricmp(bg.c_str(), http) != 0) {
        return URL_FORMAT_WRONG;
    }

    return OK;
}



