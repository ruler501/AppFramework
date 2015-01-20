#ifndef _GOOGLEPLUS_H_
#define _GOOGLEPLUS_H_

#include "main.h"
#include "gpg/gpg.h"
#include "gpg/status.h"

//extern std::unique_ptr<gpg::GameServices> game_services;
//extern gpg::GameServices* game_services;
extern bool gpLoggedIn;

bool initGPS();
bool gpLogin();
std::vector<std::string> gpGetFriendIds();
std::string gpGetProfileImage();
std::string gpGetName();
std::string gpGetId();

#endif // _GOOGLEPLUS_H_
