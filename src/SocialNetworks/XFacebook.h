#ifndef _XFACEBOOK_H_
#define _XFACEBOOK_H_

#include "SDL.h"
#include <vector>
#include <string>
#include "XSocialNetwork.h"

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

