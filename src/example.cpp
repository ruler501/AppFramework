#include "example.h"

/* Adapted from SDL's testspriteminimal.c */
Sprite LoadSprite(const char* file, SDL_Renderer* renderer)
{
	Sprite result;
	result.texture = NULL;
	result.position.w = 0;
	result.position.h = 0;
	result.position.x = 0;
	result.position.y = 0;
	result.scale = 1.f;
	result.angle = 0.f;

    SDL_Surface* temp;

    /* Load the sprite image */
    temp = SDL_LoadBMP(file);
    if (temp == NULL)
	{
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return result;
    }
    result.position.w = temp->w;
    result.position.h = temp->h;

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

bool Sprite::draw(){
	SDL_Rect destRect = {position.x, position.y, static_cast<int>(scale*position.w), static_cast<int>(scale*position.h)};
	SDL_Rect srcRect = {0, 0, position.w, position.h};
	/* Blit the sprite onto the screen */
	SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, angle, NULL, SDL_FLIP_NONE);
	return true;
}

bool InputBox::draw(){
    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
    SDL_RenderFillRect(renderer, &position);

    if(font){
        SDL_Surface *surf = TTF_RenderText_Blended(font, (text + composition).c_str(), textcolor);
        if(surf){
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_FreeSurface(surf);
            if(texture){
                int iW, iH;
                SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
                SDL_Rect destRect = {position.x, position.y, std::min(iW, position.w), std::min(iH, position.h)};
                SDL_Rect srcRect = {0, 0, std::min(iW, position.w), std::min(iH, position.h)};
                SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
                SDL_DestroyTexture(texture);
                texture = NULL;
            }
        }
        return true;
    }
    return false;
}

bool KeyEventProcessor::process(SDL_Event &event){
    switch(event.key.keysym.sym){
    case SDLK_AC_BACK:
        myView->done = true;
        break;
    case SDLK_BACKSPACE:
        if(myView->myInput.composition.length()) myView->myInput.composition.erase(myView->myInput.composition.end());
        else if(myView->myInput.text.length()) myView->myInput.text.erase(myView->myInput.text.end());
        break;
    case SDLK_RETURN:
        myView->myInput.text += '\n';
        break;
    }
    return true;
}

SpriteView::SpriteView(EventController* controller)
	: myController(controller),
	sprite(LoadSprite("image.bmp", renderer)), ccol(0), vel({0,0}),
	font(TTF_OpenFont(std::string("rimouski.ttf").c_str(), 48)),
	music(Mix_LoadMUS("music.ogg"))
    {
	    accelerometer = SDL_JoystickOpen(0);
	    if (accelerometer == NULL) done = true;
	    SDL_GetWindowSize(window, &w, &h);
	    for(int i=0; i < 8; i++) colors[i] = i*32;
	    myInput.position = {w/2, h/4, w/4, h/8};
    }

SpriteView::~SpriteView(){
    TTF_CloseFont(font);
    font = nullptr;
	SDL_DestroyTexture(sprite.texture);
	sprite.texture = nullptr;
	done = true;
}

bool SpriteView::activate(){
    myEvents.push_back(std::make_shared<KeyEventProcessor>(myController, this));
    myEvents.push_back(std::make_shared<FMotionEventProcessor>(myController, this));
    myEvents.push_back(std::make_shared<InputEventProcessor>(myController, this));
    myEvents.push_back(std::make_shared<EditEventProcessor>(myController, this));
    myEvents.push_back(std::make_shared<MGestureEventProcessor>(myController, this));
    myEvents.push_back(std::make_shared<QuitEventProcessor>(myController, this));
    myEvents.push_back(std::make_shared<FDownEventProcesor>(myController, this));
    SDL_JoystickEventState(SDL_QUERY);
    Mix_PlayMusic(music, -1);
    activated = true;
    return true;
}

bool SpriteView::updateWorld(){
    SDL_JoystickUpdate();
    vel[0] = 90*(float)SDL_JoystickGetAxis(accelerometer, 0)/65536.f;
    vel[1] = 90*(float)SDL_JoystickGetAxis(accelerometer, 1)/65536.f;
    sprite.position.x += vel[0];
    sprite.position.y += vel[1];
    if (sprite.position.x + sprite.position.w*sprite.scale > w){
        sprite.position.x = w - sprite.position.w*sprite.scale;
        if (ccol < 7) ++ccol;
    }
    if (sprite.position.x < 0){
        sprite.position.x = 0;
        if (ccol > 0) --ccol;
    }
    if (sprite.position.y + sprite.position.h*sprite.scale > h){
        sprite.position.y = h - sprite.position.h*sprite.scale;
        if (ccol < 7) ++ccol;
    }
    if (sprite.position.y < 0){
        sprite.position.y = 0;
        if(ccol > 0) --ccol;
    }
    return !done;
}

bool SpriteView::drawWorld(){
    SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
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

    myInput.draw();
    sprite.draw();

SDL_SetRenderDrawColor(renderer, colors[ccol], colors[ccol], colors[ccol], 0xFF);
    return !done;
}

bool SpriteView::deactivate(){
    for(auto& a : myEvents) a->deactivate();
    return true;
}
