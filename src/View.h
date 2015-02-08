#ifndef _VIEW_H_
#define _VIEW_H_

#include <vector>
#include "EventProcessor.h"

//! Views are a generic version of the update and draw functions
/**
 Can be used to specify different viewpoints to the app or completely different apps.
 You should specify your event handlers in \a activate()
 */
class View {
protected:
    std::vector<std::shared_ptr<EventProcessor> > myEvents;

public:
    bool activated, done;

    View(): activated(false), done(false) {}

    //! Sets the world state to the start position and registers event processors
    virtual bool activate() = 0;
    //! Updates the world state, but doesn't output to the screen/audio
    virtual bool updateWorld() = 0;
    //! Outputs the world state to screen/audio
    virtual bool drawWorld() = 0;
    //! Deactivates the View, should turn off all of its events.
    virtual bool deactivate() = 0;
};


class Overlay : public View{
};

#endif
