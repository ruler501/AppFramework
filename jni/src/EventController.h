#ifndef _EVENTCONTROLLER_H_
#define _EVENTCONTROLLER_H_ 1

#include <map>
#include <vector>
#include <string>
#include <mutex>

#include "SDL.h"

class EventProcessor;

class EventController{
protected:
	std::map<SDL_EventType, std::vector<EventProcessor*> > myEvents;
//	std::unique_lock<std::mutex> eLock;

public:
	bool process(SDL_Event event);
	bool registerEvent(EventProcessor* processor, SDL_EventType event);
	bool deactivateEvent(EventProcessor* processor, SDL_EventType event);
};

#endif
