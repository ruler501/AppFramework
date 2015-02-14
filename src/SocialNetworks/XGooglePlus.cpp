#include "XGooglePlus.h"

bool XGooglePlus::login(){
    isFinished = false;

    if(!loggedIn) game_services->StartAuthorizationUI();

    if(!isFinished) SDL_CondWait(authCond, authMutex);

    return (loggedIn = isLoggedIn);
}

std::vector<std::string> XGooglePlus::getFriendIDs(){
    if(!loggedIn) login();

    std::vector<std::string> friendIDs;

    gpg::PlayerManager::FetchListResponse fResponse = game_services->Players().FetchInvitableBlocking();

    for(auto& p : fResponse.data){
        friendIDs.push_back(p.Id());
    }

    return friendIDs;
}

std::string XGooglePlus::getProfileImage(){
    if(!loggedIn) login();

    gpg::PlayerManager::FetchSelfResponse usResponse = game_services->Players().FetchSelfBlocking();

    gpg::Player us = usResponse.data;

    if(!us.Valid()) SDL_Log("Invalid player");

    return us.AvatarUrl(gpg::ImageResolution::HI_RES);
}

std::string XGooglePlus::getProfileName(){
    if(!loggedIn) login();

    gpg::PlayerManager::FetchSelfResponse usResponse = game_services->Players().FetchSelfBlocking();

    gpg::Player us = usResponse.data;

    if(!us.Valid()) SDL_Log("Invalid player");

    return us.Name();
}


std::string XGooglePlus::getProfileID(){
    if(!loggedIn) login();

    gpg::PlayerManager::FetchSelfResponse usResponse = game_services->Players().FetchSelfBlocking();

    gpg::Player us = usResponse.data;

    if(!us.Valid()) SDL_Log("Invalid player");

    return us.Id();
}
