#ifndef _MAIN_H_
#define _MAIN_H_

#include "SDL_mixer.h"

#include "EventController.h"
#include "EventProcessor.h"
#include "View.h"


typedef struct Sprite
{
	SDL_Texture* texture;
	Uint16 w;
	Uint16 h;
	Uint16 x;
	Uint16 y;
} Sprite;

SDL_Window *window;
SDL_Renderer *renderer;

class SpriteView : public View {
public:
	EventController* myController;
	bool done;
	Sprite sprite;
	int x, y, w, h, vel[2];
	float angle;
	TTF_Font *font;
	SDL_Joystick *accelerometer;
	std::string text, composition;
	Mix_Music *music;

	SpriteView(EventController* controller);
	~SpriteView();

    virtual bool activate();
    virtual bool updateWorld();
    virtual bool drawWorld();
    virtual bool deactivate();
};

class KeyEventProcessor : public EventProcessor{
protected:
	SpriteView* myView;

public:
	KeyEventProcessor(EventController* controller, SpriteView* that)
	: EventProcessor(controller, SDL_KEYDOWN), myView(that)
	{}

	virtual ~KeyEventProcessor() {}

	virtual bool process(SDL_Event &event);
};

class QuitEventProcessor : public EventProcessor{
private:
	SpriteView* myView;
public:
	QuitEventProcessor(EventController* controller, SpriteView* that)
	: EventProcessor(controller, SDL_QUIT), myView(that)
	{}

	virtual ~QuitEventProcessor() {}

	virtual bool process(SDL_Event &event) { myView->done = true; }
};

class InputEventProcessor : public EventProcessor{
protected:
	SpriteView* myView;

public:
	InputEventProcessor(EventController* controller, SpriteView* that)
	: EventProcessor(controller, SDL_TEXTINPUT), myView(that)
	{}

	virtual ~InputEventProcessor() {}

	virtual bool process(SDL_Event &event) {
		myView->text += event.text.text;
		myView->composition = std::string();
	}
};

class EditEventProcessor : public EventProcessor{
protected:
	SpriteView* myView;

public:
	EditEventProcessor(EventController* controller, SpriteView* that)
	: EventProcessor(controller, SDL_TEXTEDITING), myView(that)
	{}

	virtual ~EditEventProcessor() {}

	virtual bool process(SDL_Event &event) { myView->composition = std::string(event.edit.text); }
};

class MGestureEventProcessor : public EventProcessor{
protected:
	SpriteView* myView;

public:
	MGestureEventProcessor(EventController* controller, SpriteView* that)
	: EventProcessor(controller, SDL_MULTIGESTURE), myView(that)
	{}

	virtual ~MGestureEventProcessor() {}

	virtual bool process(SDL_Event &event) { myView->angle += 90*event.mgesture.dTheta; }
};

class FMotionEventProcessor : public EventProcessor{
protected:
	SpriteView* myView;

public:
	FMotionEventProcessor(EventController* controller, SpriteView* that)
	: EventProcessor(controller, SDL_FINGERMOTION), myView(that)
	{}

	virtual ~FMotionEventProcessor() {}

	virtual bool process(SDL_Event &event){
		myView->x += myView->w*event.tfinger.dx;
		myView->y += myView->h*event.tfinger.dy;
	}
};

class FDownEventProcesor : public EventProcessor{
protected:
	SpriteView *myView;

public:
	FDownEventProcesor(EventController* controller, SpriteView* that)
	: EventProcessor(controller, SDL_FINGERDOWN), myView(that)
	{}

//	virtual ~FDownEventProcessor() {}

	virtual bool process(SDL_Event &event){

		if (abs(event.tfinger.x-(myView->x+myView->sprite.w/2)) > 2*myView->sprite.w || abs(event.tfinger.y-myView->y+myView->sprite.h/2) > 2*myView->sprite.h){
			if(SDL_IsTextInputActive()) SDL_StopTextInput();
			else SDL_StartTextInput();
		}
	}
};

#endif // _MAIN_H_
