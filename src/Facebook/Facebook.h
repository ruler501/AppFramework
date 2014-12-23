#ifndef _FACEBOOK_H_
#define _FACEBOOK_H_

#include "SDL.h"
#include <vector>

struct friends{
    SDL_cond* waiting;
    std::vector<int> friendIDs;
};

struct goodCall{
    SDL_cond* waiting;
    bool success;
};

#endif // _FACEBOOK_H_

