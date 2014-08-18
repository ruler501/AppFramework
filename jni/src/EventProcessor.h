#ifndef _EVENTPROCESSOR_H_
#define _EVENTPROCESSOR_H_ 1

#include <vector>
#include <string>

#include "SDL.h"

#include "EventController.h"

class EventProcessor{
protected:
	EventController* myController;
	SDL_EventType myEvent;

public:
	EventProcessor() : myController(nullptr), myEvent(SDL_FIRSTEVENT) {}
	EventProcessor(EventController* control) : myController(control), myEvent(SDL_FIRSTEVENT) {}
	EventProcessor(EventController* control, SDL_EventType event) : myController(control), myEvent(event) { myController->registerEvent(this, myEvent); }

	virtual ~EventProcessor() { myController->deactivateEvent(this, myEvent); }

	virtual bool getRegistered() { return myController->registerEvent(this, myEvent); }
	virtual bool process(SDL_Event &event) = 0;
	virtual bool operator()(SDL_Event &event) { process(event); }
	virtual bool deactivate() { return myController->deactivateEvent(this, myEvent); }
};

#endif
