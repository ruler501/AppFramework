//#include<async>
#include<algorithm>

#include "EventController.h"
#include "EventProcessor.h"

bool EventController::process(SDL_Event event){
	bool result = false;
	std::vector<EventProcessor*> thisEvent;
	//eLock.lock();
    thisEvent = myEvents[static_cast<SDL_EventType>(event.type)];
	//eLock.unlock();
	for(auto& ep : thisEvent){
		result |= ep->process(event);
	}
	return result;
}

bool EventController::registerEvent(EventProcessor* processor, SDL_EventType event){
	//eLock.lock()
	myEvents[event].push_back(processor);
	//eLock.unlock()
	return true;
}

bool EventController::deactivateEvent(EventProcessor* processor, SDL_EventType event){
	//eLock.lock();
	std::vector<EventProcessor*> thisEvent;
	try{
		thisEvent = myEvents.at(event);
	} catch (const std::out_of_range &oor){
//		eLock.unlock();
		return false;
	}
	//eLock.unlock();
	thisEvent.erase(std::remove(thisEvent.begin(), thisEvent.end(), processor), thisEvent.end());
	//eLock.lock();
	myEvents[event] = thisEvent;
//	eLock.unlock();
	return true;
}

bool EventController::clearEvents(){
    /*for(auto& ev : myEvents){
		for(auto& e : ev.second){
			delete e;
		}
    }*/
    myEvents.clear();
    return true;
}
