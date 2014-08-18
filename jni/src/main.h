#ifndef _MAIN_H_
#define _MAIN_H_


#include "EventController.h"
#include "EventProcessor.h"
#include "View.h"


typedef struct Sprite
{
	SDL_Texture* texture;
	Uint16 w;
	Uint16 h;
} Sprite;

SDL_Window *window;
SDL_Renderer *renderer;

class KeyEventProcessor : public EventProcessor{
protected:
	std::string *text, *composition;
	bool *done;

public:
	KeyEventProcessor(EventController* controller, std::string &textRef, std::string &compRef, bool &doneRef)
	: EventProcessor(controller, SDL_TEXTINPUT), text(&textRef), composition(&compRef), done(&doneRef)
	{}

	virtual ~KeyEventProcessor() {}

	virtual bool process(SDL_Event &event);
};

class QuitEventProcessor : public EventProcessor{
public:
	bool *done;
	QuitEventProcessor(EventController* controller, bool &doneRef)
	: EventProcessor(controller, SDL_QUIT), done(&doneRef)
	{}

	virtual ~QuitEventProcessor() {}

	virtual bool process(SDL_Event &event) { *done = true; }
};

class InputEventProcessor : public EventProcessor{
protected:
	std::string *text, *composition;

public:
	InputEventProcessor(EventController* controller, std::string &textRef, std::string &compRef)
	: EventProcessor(controller, SDL_TEXTINPUT), text(&textRef), composition(&compRef)
	{}

	virtual ~InputEventProcessor() {}

	virtual bool process(SDL_Event &event) {
		*text += event.text.text;
		*composition = std::string();
	}
};

class EditEventProcessor : public EventProcessor{
protected:
	std::string *composition;

public:
	EditEventProcessor(EventController* controller, std::string &compRef)
	: EventProcessor(controller, SDL_TEXTINPUT), composition(&compRef)
	{}

	virtual ~EditEventProcessor() {}

	virtual bool process(SDL_Event &event) { *composition = std::string(event.edit.text); }
};

class MGestureEventProcessor : public EventProcessor{
protected:
	float* angle;

public:
	MGestureEventProcessor(EventController* controller, float &angleRef)
	: EventProcessor(controller, SDL_MULTIGESTURE), angle(&angleRef)
	{}

	virtual ~MGestureEventProcessor() {}

	virtual bool process(SDL_Event &event) { *angle += 180*event.mgesture.dTheta; }
};

class FMotionEventProcessor : public EventProcessor{
protected:
	int *x, *y;
	int w, h;

public:
	FMotionEventProcessor(EventController* controller, int &xRef, int &yRef, int wi, int hi)
	: EventProcessor(controller, SDL_FINGERMOTION), x(&xRef), y(&yRef), w(wi), h(hi)
	{}

	virtual ~FMotionEventProcessor() {}

	virtual bool process(SDL_Event &event){
		*x += w*event.tfinger.dx;
		*y += h*event.tfinger.dy;
	}
};

class SpriteView : public View {
protected:
	EventController* myController;
	bool done;
	Sprite sprite;
	int x, y, w, h;
	float angle, vel[2];
	TTF_Font *font;
	SDL_Joystick *accelerometer;
	std::string text, composition;

public:
	SpriteView(EventController* controller);

    virtual bool activate();
    virtual bool updateWorld();
    virtual bool drawWorld();
    virtual bool deactivate();
};

#endif // _MAIN_H_
