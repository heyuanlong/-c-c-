#ifndef __MS_TIMERCONTENT_H__
#define __MS_TIMERCONTENT_H__

#include <MsTimerDefines.h>

namespace matchvs
{
	typedef struct MsTimerContent{
		TimerExpiry *cb;
		TimerRollback* rb;
		void *userData;
		int flag;
		long intervalTime;
	}MsTimerContent;

	class MsTimer{
		public:
			MsTimer(){}
			~MsTimer(){}

			msTimerId id;
			long evTimeout;
		public:
			bool operator < (const MsTimer& b)
			{ return evTimeout < b.evTimeout ; }
			bool operator <= (const MsTimer& b)
			{ return evTimeout <= b.evTimeout ; }
			bool operator > (const MsTimer& b)
			{ return evTimeout > b.evTimeout ; }
			bool operator >= (const MsTimer& b)
			{ return evTimeout >= b.evTimeout ; }

			bool operator == (const MsTimer& b)
			{ return id == b.id ; }
	};
};

#endif