#ifndef _EVENTCONTROLLER_H_
#define _EVENTCONTROLLER_H_

#include <map>
#include <vector>
#include <string>
#include <mutex>

#include "SDL.h"

class EventProcessor;

//! Controls the event processors, effectively a singleton
class EventController{
protected:
	std::map<SDL_EventType, std::vector<EventProcessor*> > myEvents;
//	std::unique_lock<std::mutex> eLock;

public:
	//! passes the event on to the processors registered for it.
	bool process(SDL_Event event);
	//! Registers a new processor
	bool registerEvent(EventProcessor* processor, SDL_EventType event);
	//! deactivates a given processor
	bool deactivateEvent(EventProcessor* processor, SDL_EventType event);
	//! delete all events
	bool clearEvents();
};

#endif
