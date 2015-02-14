#ifndef _XGOOGLEPLUS_H_
#define _XGOOGLEPLUS_H_

#include "XSocialNetwork.h"
#include "main.h"
#include "gpg/gpg.h"
#include "gpg/status.h"

class XGooglePlus : public XSocialNetwork{
private:
    gpg::GameServices* game_services;
    SDL_cond* authCond;
    SDL_mutex* authMutex;
    bool isLoggedIn, isFinished;

public:
    XGooglePlus();

    bool login();
    std::vector<std::string> getFriendIDs();
    std::string getProfileID();
    std::string getProfileImage();
    std::string getProfileName();
};
#endif // _GOOGLEPLUS_H_
