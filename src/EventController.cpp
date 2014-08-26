//#include<async>
#include<algorithm>

#include "EventController.h"
#include "EventProcessor.h"

bool EventController::process(SDL_Event event){
	bool result = true;
	std::vector<std::shared_ptr<EventProcessor> > thisEvent;
	//eLock.lock();
	try{
		thisEvent = myEvents.at(event.type);
	} catch (const std::out_of_range &oor){
//		eLock.unlock();
		return false;
	}
	//eLock.unlock();
	for(auto& ep : thisEvent){
		result = result && ep->process(event);
	}
	return result;
}

bool EventController::registerEvent(EventProcessor* processor, SDL_EventType event){
	//eLock.lock()
	try{
		myEvents.at(event).push_back(std::shared_ptr<EventProcessor>(processor));
	} catch (const std::out_of_range &oor){
		myEvents[event] = std::vector<std::shared_ptr<EventProcessor> >(1,std::shared_ptr<EventProcessor>(processor));
	}
	//eLock.unlock()
	return true;
}

bool EventController::deactivateEvent(EventProcessor* processor, SDL_EventType event){
	//eLock.lock();
	std::vector<std::shared_ptr<EventProcessor> > thisEvent;
	try{
		thisEvent = myEvents.at(event);
	} catch (const std::out_of_range &oor){
//		eLock.unlock();
		return false;
	}
	//eLock.unlock();
	thisEvent.erase(std::remove(thisEvent.begin(), thisEvent.end(), std::shared_ptr<EventProcessor>(processor)), thisEvent.end());
	//eLock.lock();
	myEvents[event] = thisEvent;
//	eLock.unlock();
	return true;
}
