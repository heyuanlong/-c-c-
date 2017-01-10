#include <MatchVSTimeManager.h>
#include <MsTimeOps.h>
#include <MsAutoLocker.h>


#define RANDOM_ID_SCOPE 1000

namespace matchvs
{
	static MatchVSTimeManager* pInstance = NULL;

	MatchVSTimeManager::MatchVSTimeManager()
	{
		mError = 0;
		int sec = getCurrentSecond();
		srand(sec);	
	}

	MatchVSTimeManager* MatchVSTimeManager::getInstance()
	{
		static MatchVSTimeManager obj;
		return &obj;
	}

	MatchVSTimeManager::~MatchVSTimeManager()
	{

	}

	msTimerId MatchVSTimeManager::addTimer(ITimerVal interval,
										TimerExpiry *cb,
										TimerRollback* rb,
										void *userData,
										Int32 &error,
										Int32 flag)
	{

		if(cb == NULL || interval.itInterval <= 0 || interval.itValue < 0){
			error = MS_TIMER_ERROR_PARA;
			return MS_FAIL;
		}

		MsTimerContent *pTemp = new MsTimerContent;
		pTemp->cb = cb;
		pTemp->rb = rb;
		pTemp->userData = userData;
		pTemp->flag = flag;
		pTemp->intervalTime = interval.itInterval;
		
		//add to map
		std::pair<std::map<msTimerId, MsTimerContent*>::iterator,bool> ret;
		msTimerId tempid;
		do{
			tempid = generateMsTimerId();
			mLockData.lock();
			ret = eventMap.insert( std::pair<msTimerId, MsTimerContent*>(tempid,pTemp) );
			mLockData.unLock();
		}while(ret.second == false);
	
		MsTimer tempEvent;					//add to minheap
		tempEvent.id = tempid;
		tempEvent.evTimeout = interval.itValue + getCurrentSecond();
		mLockData.lock();
		msHeap.insert(tempEvent);
		mLockData.unLock();

		return tempid;
	}
	msTimerId MatchVSTimeManager::removeTimer(msTimerId id)
	{
		std::map<msTimerId, MsTimerContent*>::iterator it;
		MsTimerContent tempContent;
		long currentTime = getCurrentSecond();

		mLockData.lock();
		it = eventMap.find(id);
  		if (it == eventMap.end()){
  			mLockData.unLock();
  			return MS_FAIL;
  		}
  		tempContent = *(it->second);
		MsTimer tempEvent;					//remove from minheap
  		tempEvent.id = id;
  		msHeap.remove(tempEvent);
		
  		delete it->second;					//remove from map
  		eventMap.erase(it);
  		mLockData.unLock();

		if (tempContent.rb != NULL){
  			tempContent.rb(tempEvent.id,currentTime,tempContent.userData,mError);
  		}

  		return id;
	}

	Int32 MatchVSTimeManager::getTopExpire()
	{
		MsTimer tempEvent;
		MsAutoLock objLock(mLockData);
		if (msHeap.size() <= 0){
			return -1;
		}
		tempEvent = msHeap.getTop();
		return tempEvent.evTimeout - getCurrentSecond();
	}

	void MatchVSTimeManager::runTimerEvent()
	{		
		MsTimer tempEvent;
		std::map<msTimerId, MsTimerContent*>::iterator it;
		std::map<msTimerId, MsTimerContent*>::iterator itEnd;
		MsTimerContent tempContent;
		bool isHave = false;
		bool isRemove = false;
		long currentTime = getCurrentSecond();

		while(true)
		{
			isHave = false;
			isRemove = false;
			mLockData.lock();
			if (msHeap.size() <= 0){
				mLockData.unLock();
				return ;
			}
			tempEvent = msHeap.getTop();
			if( (tempEvent.evTimeout - currentTime) > 0 ){
				mLockData.unLock();
				return ;
			}

			it = eventMap.find(tempEvent.id);
			itEnd = eventMap.end();
			
	  		if (it != itEnd ){ 		
	  			tempContent	= *(it->second);
				isHave = true;
  				if (it->second->flag == TIMER_REUSER){ 	//Whether to reuse							
  					msHeap.remove(tempEvent);			//remove from heap  -> add to heap
  					tempEvent.evTimeout = it->second->intervalTime + currentTime;
					msHeap.insert(tempEvent);
  				}else{
  					isRemove = true;
														//remove from heap and map
			  		msHeap.remove(tempEvent);
			  		delete it->second;
			  		eventMap.erase(it);
  				}
	  		}
	  		mLockData.unLock();

			// callback
			if (isHave == true){
				tempContent.cb(tempEvent.id,currentTime,tempContent.userData, mError);
				if (isRemove == true && tempContent.rb != NULL)
				{
					tempContent.rb(tempEvent.id,currentTime,tempContent.userData, mError);
				}
			}
  		}

	}

	msTimerId MatchVSTimeManager::generateMsTimerId()
	{	
		int nums = RANDOM_ID_SCOPE;
		if (nums > 1000){
			nums = 1000;
		}

		return (rand() % nums) * (rand() % nums);		//一百万
	}

}