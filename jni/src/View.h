#include <vector>
#include "EventProcessor.h"

class View {
protected:
    std::vector<EventProcessor*> myEvents;

public:
    virtual bool activate() = 0;
    virtual bool updateWorld() = 0;
    virtual bool drawWorld() = 0;
    virtual bool deactivate() = 0;
};
