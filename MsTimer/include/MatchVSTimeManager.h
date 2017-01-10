#ifndef __MATCHVS_TIME_MANAGER_H__
#define __MATCHVS_TIME_MANAGER_H__

#include <MsBase.h>
#include <MsCustomError.h>
#include <MsMinHeap.h>
#include <MsTimerContent.h>
#include <MsMutexLocker.h>
#include <MsSafeSTL.h>

#define TIMER_REUSER 1

namespace matchvs
{	
	class MatchVSTimeManager
	{
		public:
			static MatchVSTimeManager* getInstance();
			~MatchVSTimeManager();
		public:
			msTimerId addTimer(ITimerVal interval,
										TimerExpiry *cb,
										TimerRollback* rb,
										void *userData,
										Int32 &error,
										Int32 flag = TIMER_REUSER
										);
			msTimerId removeTimer(msTimerId id);

			Int32 getTopExpire();
			void runTimerEvent();
			
		private:
			MatchVSTimeManager();
			msTimerId generateMsTimerId();

			SafeMap<msTimerId, MsTimerContent*> eventMap;
			MsMinHeap<MsTimer> msHeap;
			MsMutexLocker mLockData;

			Int32 mError;
	};
};

#endif