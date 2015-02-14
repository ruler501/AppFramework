#ifndef _XFACEBOOK_H_
#define _XFACEBOOK_H_

#include "SDL.h"
#include <vector>
#include <string>
#include "XSocialNetwork.h"

struct friends{
    friends() : completed(false), waiting(SDL_CreateCond()), friendIDs{} {}
    bool completed;
    SDL_cond* waiting;
    std::vector<std::string> friendIDs;
};

struct goodCall{
    goodCall() : completed(false), waiting(SDL_CreateCond()), success(false) {}
    bool completed;
    SDL_cond* waiting;
    bool success;
};

struct sReturn{
    sReturn() : completed(false), waiting(SDL_CreateCond()) {}
    bool completed;
    SDL_cond* waiting;
    std::string result;
};

class XFacebook : public XSocialNetwork{
public:
    XFacebook();

    bool login();
    std::vector<std::string> getFriendIDs();
    std::string getProfileID();
    std::string getProfileImage();
    std::string getProfileName();
};

#endif // _FACEBOOK_H_

