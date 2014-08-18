#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>
#include <deque>
#include <sstream>

#include "SDL.h"
#include "SDL_ttf.h"

#include "main.h"

std::deque<View*> views;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

/* Adapted from SDL's testspriteminimal.c */
Sprite LoadSprite(const char* file, SDL_Renderer* renderer)
{
	Sprite result;
	result.texture = NULL;
	result.w = 0;
	result.h = 0;

    SDL_Surface* temp;

    /* Load the sprite image */
    temp = SDL_LoadBMP(file);
    if (temp == NULL)
	{
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return result;
    }
    result.w = temp->w;
    result.h = temp->h;

    /* Create texture from the image */
    result.texture = SDL_CreateTextureFromSurface(renderer, temp);
    if (!result.texture) {
        fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(temp);
        return result;
    }
    SDL_FreeSurface(temp);

    return result;
}

void draw(const Sprite sprite, int x, int y, int angle)
{
	SDL_Rect destRect = {x, y, sprite.w, sprite.h};
	SDL_Rect srcRect = {0, 0, sprite.w, sprite.h};
	/* Blit the sprite onto the screen */
	SDL_RenderCopyEx(renderer, sprite.texture, &srcRect, &destRect, angle, NULL, SDL_FLIP_NONE);
}

bool KeyEventProcessor::process(SDL_Event &event){
    switch(event.key.keysym.sym){
    case SDLK_AC_BACK:
        *done = true;
        break;
    case SDLK_BACKSPACE:
        if(composition->length()) composition->erase(composition->end());
        else if(text->length()) text->erase(text->end());
        break;
    case SDLK_RETURN:
        *text += '\n';
        break;
    }
}

SpriteView::SpriteView(EventController* controller)
	: myController(controller), sprite(LoadSprite("image.bmp", renderer)), x(0), y(0), angle(0),
	vel({0,0}), accelerometer(SDL_JoystickOpen(0)), done(false), font(TTF_OpenFont(std::string("rimouski.ttf").c_str(), 48)),
	music(Mix_LoadMUS("music.ogg"))
	{
	    SDL_GetWindowSize(window, &w, &h);
    }

SpriteView::~SpriteView(){
    for(auto &a : myEvents) delete a;
}

bool SpriteView::activate(){
    myEvents.push_back(new KeyEventProcessor(myController, text, composition, done));
    myEvents.push_back(new FMotionEventProcessor(myController, x, y, w, h));
    myEvents.push_back(new InputEventProcessor(myController, text, composition));
    myEvents.push_back(new EditEventProcessor(myController, composition));
    myEvents.push_back(new MGestureEventProcessor(myController, angle));
    myEvents.push_back(new QuitEventProcessor(myController, done));
    SDL_JoystickEventState(SDL_QUERY);
    return true;
}

bool SpriteView::updateWorld(){
    SDL_JoystickUpdate();
    vel[0] += SDL_JoystickGetAxis(accelerometer, 0);
    vel[1] += SDL_JoystickGetAxis(accelerometer, 1);
    x += vel[0];
    y += vel[1];
    if (x + sprite.w > w || x < 0) vel[0] = -vel[0]/2;
    if (y + sprite.h > h || y < 0) vel[1] = -vel[1]/2;
    return !done;
}

bool SpriteView::drawWorld(){
    /* Draw a gray background */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Color color = { 255, 255, 255, 255 };
    if(font){
        SDL_Surface *surf = TTF_RenderText_Blended(font, (text + composition).c_str(), color);
        if(surf){
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_FreeSurface(surf);
            if(texture){
                int iW, iH;
                SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
                SDL_Rect destRect = {w/2 - iW/2, h/2 - iH/2, iW, iH};
                SDL_RenderCopy(renderer, texture, NULL, &destRect);
                SDL_DestroyTexture(texture);
                texture = NULL;
            }
        }
    }

    draw(sprite, x, y, angle);

    /*Update the screen!*/
    SDL_RenderPresent(renderer);

    if (x>w/2) Mix_PlayMusic(music,0);

    return !done;
}

bool SpriteView::deactivate(){
    TTF_CloseFont(font);
	SDL_DestroyTexture(sprite.texture);
	sprite.texture = NULL;
}

using namespace std;

int main(int argc, char *argv[])
{
    if(SDL_CreateWindowAndRenderer(0, 0, 0, &window, &renderer) < 0)
        exit(2);

	TTF_Init();

	int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
    int audio_channels = 2;
    int audio_buffers = 4096;

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
        printf("Unable to open audio!\n");
        exit(1);
    }

    /* Main render loop */
    SDL_Event event;
    EventController ourController;
	SpriteView* tempView = new SpriteView(&ourController);
	views.push_back(tempView);
	//if (font == NULL) done = true;
	int millis = SDL_GetTicks();
	while(!views.empty()){
        View* current = views[0];
        current->activate();
        bool cont = true;
        while(cont){
            millis = SDL_GetTicks();
            while(SDL_PollEvent(&event)){
                ourController.process(event);
            }
            cont &= current->updateWorld();
            cont &= current->drawWorld();
            SDL_Delay(17-millis+SDL_GetTicks());
        }
        current->deactivate();
        views.pop_front();
	}

	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;

	TTF_Quit();

    exit(0);
}
