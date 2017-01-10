#ifndef __MS_TIMER_DEFINES_H__
#define __MS_TIMER_DEFINES_H__

namespace matchvs
{
	typedef unsigned long long msTimerId;

	/**
	* The type of callback function to be called by timer scheduler when a timer
	* has expired.
	*
	* @param id					The timer id.
	* @param trigger_time 		The trigger time of timer.
	* @param user_data 			The user data.
	* @param len				The length of user data.
	*/
	typedef int TimerExpiry(msTimerId id, long triggerTime, void* userData, int& error);

	/**
	* The type of rollback function to be called by timer scheduler when a timer
	* has removed.
	*
	* @param id					The timer id.
	* @param trigger_time 		The trigger time of timer.
	* @param user_data 			The user data.
	* @param len				The length of user data.
	*/
	typedef int TimerRollback(msTimerId id, long triggerTime, void* useData, int& error);


	/**
	* Interval timer value
	*/
	typedef struct {
		long itInterval;		/* next value */
		long itValue;			/* current value */
	} ITimerVal;
};




#endif