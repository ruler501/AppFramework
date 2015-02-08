#ifndef _EVENTPROCESSOR_H_
#define _EVENTPROCESSOR_H_

#include <vector>
#include <string>

#include "SDL.h"

#include "EventController.h"

//! Generic Event Processor
/**
 * Processes events in a generic fashion. Is 'plug and play'
 * on creation registers with its controller and waits for events
 **/
class EventProcessor{
protected:
	EventController* myController;
	SDL_EventType myEvent;

public:
	//! Generic constructor to make std::vector happy
	EventProcessor() : myController(nullptr), myEvent(SDL_FIRSTEVENT) {}
	//! Actually useful constructor, creates and registers the processor
	/**
	 You are advised to call to this constructor from all inherited classes.
	 @param control The controller the processor should register with
	 @param event The event type the processor should work with
	 */
	EventProcessor(EventController* control, SDL_EventType event) : myController(control), myEvent(event) { myController->registerEvent(this, myEvent); }

	//! unregisters the processor
	virtual ~EventProcessor() { myController->deactivateEvent(this, myEvent); }

	//! Registers the processor, almost never called since the constructor does the same thing
	virtual bool getRegistered() { return myController->registerEvent(this, myEvent); }
	//! Does the actual hard work of processing events and changing the View's state
	//! @param event The event the processor is to process
	virtual bool process(SDL_Event &event) = 0;
	//! Alias to \a process
	virtual bool operator()(SDL_Event &event) { return process(event); }
	//! Deactivates the processor, unlikely to be called much since destructor does it also.
	virtual bool deactivate() { return myController->deactivateEvent(this, myEvent); }
};

#endif
