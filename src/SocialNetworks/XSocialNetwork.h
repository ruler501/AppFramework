#ifndef _XSOCIALNETWORK_H_
#define _XSOCIALNETWORK_H_

#include <vector>
#include <string>
#include "SDL.h"

class XSocialNetwork{
protected:
    bool loggedIn;
    SDL_Texture* icon;

public:
    XSocialNetwork() : loggedIn(false), icon(nullptr) {}

    virtual bool isLogin() {return loggedIn;}
    virtual bool login() = 0;
    virtual std::vector<std::string> getFriendIDs() = 0;
    virtual std::string getProfileID() = 0;
    virtual std::string getProfileImage() = 0;
    virtual std::string getProfileName() = 0;
    SDL_Texture* getIcon() {return icon;}
};

#endif // _SOCIALNETWORK_H_
