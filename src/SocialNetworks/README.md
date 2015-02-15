# Social Networks

## How to Use This
    XGooglePlus ourNetwork;//Could also be XFacebook
    if(!ourNetwork.isLogin())
        ourNetwork.login();
    std::vector<std::string> friendIDs = ourNetwork.getFriendIDs();
    std::string profileID = ourNetwork.getProfileID();
    std::string profileImageURL = ourNetowrk.getProfileImage();
    std::string userName = ourNetwork.getProfileName();
    SDL_Texture* icon = ourNetwork.getIcon();

## Implemented Networks
Currently only GooglePlus and and Facebook are implemented, will accept pull requests adding others

## Implementation

### Android

#### Facebook
Java calls using the simple-facebook-android library by sromku

#### Google Plus
Java call to initialize and then C++ calls using the google play games library

### iOS

#### Facebook
Not implemented as I don't have a device to test on.

#### Google Plus
Should work, completely untested though. I don't even know if it compiles.
