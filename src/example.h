#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include "main.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

class GUIElement{
public:
	SDL_Rect position;

	virtual bool draw() = 0;
};

class Sprite : public GUIElement{
public:
	SDL_Texture* texture;
	float angle, scale;

	virtual bool draw();
};

class InputBox : public GUIElement{
public:
	TTF_Font *font;
	std::string text;
	std::string composition;
	SDL_Color background, textcolor;

	InputBox() : font(TTF_OpenFont(std::string("rimouski.ttf").c_str(), 48)), background({0xFF, 0xFF, 0xFF, 0xFF}), textcolor({0x00, 0x00, 0x00, 0xFF}) {}
	~InputBox() {
		TTF_CloseFont(font);
		font = nullptr;
    }

	virtual bool draw();
};

class SpriteView : public View {
public:
	EventController* myController;
	Sprite sprite;
	int w, h, ccol, cdir, vel[2], colors[8];
	float angle, scale;
	TTF_Font *font;
	SDL_Joystick *accelerometer;
	std::string text, composition;
	Mix_Music *music;
	InputBox myInput;

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

	virtual bool process(SDL_Event &event) { myView->done = true;  return true;}
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
		myView->myInput.text += event.text.text;
		myView->myInput.composition = std::string();
		return true;
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

	virtual bool process(SDL_Event &event) { myView->myInput.composition = std::string(event.edit.text); return true;}
};

class MGestureEventProcessor : public EventProcessor{
protected:
	SpriteView* myView;

public:
	MGestureEventProcessor(EventController* controller, SpriteView* that)
	: EventProcessor(controller, SDL_MULTIGESTURE), myView(that)
	{}

	virtual ~MGestureEventProcessor() {}

	virtual bool process(SDL_Event &event) {
		myView->sprite.angle += 90*event.mgesture.dTheta;
 		myView->sprite.scale +=  (float)event.mgesture.dDist*myView->h/float(myView->sprite.position.w + myView->sprite.position.h);
		return true;
	}
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
		myView->sprite.position.x += myView->w*event.tfinger.dx;
		myView->sprite.position.y += myView->h*event.tfinger.dy;
		return true;
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
		SDL_Point point = {static_cast<int>(event.tfinger.x*myView->w), static_cast<int>(event.tfinger.y*myView->h)};
		SDL_Rect rect = myView->myInput.position;
		if (enclosedPoint(point, rect)){
			if(SDL_IsTextInputActive()) SDL_StopTextInput();
			else SDL_StartTextInput();
		}
		return true;
	}
};

#endif // _EXAMPLE_H_
