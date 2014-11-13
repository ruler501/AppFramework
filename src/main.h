#ifndef _MAIN_H_
#define _MAIN_H_

#include <deque>
#include <memory>
#ifdef __ANDROID_API__
    #include <jni.h>
#endif

#include "EventController.h"
#include "EventProcessor.h"
#include "View.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern std::deque<std::shared_ptr<View> > views;
extern std::deque<std::shared_ptr<Overlay> > overlays;
#ifdef __ANDROID_API__
    extern JNIEnv* env;
    extern jobject activity;
#endif
extern const char* pref_path;
extern EventController viewController;
extern EventController overlayController;
extern std::string uid;

SDL_Texture* loadImage(const char* path);
bool enclosedPoint(SDL_Point &point, SDL_Rect &rect);
std::string getUID();
template<typename T> std::string to_string(T value);

#endif // _MAIN_H_
