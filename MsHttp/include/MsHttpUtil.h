#ifndef _MSHTTP_UTILITY_H__
#define _MSHTTP_UTILITY_H__

#include <string>
#include <zlib.h>

class MsHttpUtil
{
public:
    MsHttpUtil();
    ~MsHttpUtil();

    static const int OK = 0;
    static const int URL_FORMAT_WRONG = -1;
    static const int DOMAIN_FORMAT_WRONG = -2;
    static const int DOMAIN_GET_IP_FAILED = -3;
    static const int COMPRESS_ERROR = -4;
    static const int DEPRESS_ERROR = -5;
    static const int COMPRESS_SIZE_TOO_BIG = -6;
    static const int DEPRESS_SIZE_TOO_BIG = -7;



    int compressGzipInit(unsigned char* dst, int dstsize);
    int compressGzipEnd(int &size);
    int compressGzip(const char* src, int size);

    int depressGzipInit(unsigned char* dst, int dstsize);
    int depressGzipEnd(int &size);
    int depressGzip(const char* src, int size);
    

private:

    static int  checkUrlFormat(std::string &url);

    z_stream    m_strm;
};






#endif _WHATS_UTILITY_H__
