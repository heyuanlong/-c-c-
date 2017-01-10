#ifndef __MS_SYS_UTIL_H__
#define __MS_SYS_UTIL_H__

#include <string>
#include <time.h>
#include <errno.h>

#include <MsBase.h>

namespace matchvs
{
	Int8* getErrStr(Int32 errorNo);
	Int8* getErrStr();
	Int32 getErrno();

	Int32 getWorkCwd(std::string &dir);


};

#endif