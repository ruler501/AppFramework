#include "GooglePlus.h"

//std::unique_ptr<gpg::GameServices> game_services;
gpg::GameServices* game_services;
bool gpLoggedIn = false;

SDL_cond* authCond = SDL_CreateCond();
SDL_mutex* authMutex = SDL_CreateMutex();
bool isLoggedIn = false;
bool isFinished = false;

#ifdef __ANDROID_API__
	bool initGPS(){
		//std::unique_ptr<gpg::GameServices> game_services;

		JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

		JavaVM* mJVM;
		aEnv->GetJavaVM(&mJVM);
		gpg::AndroidInitialization::JNI_OnLoad(mJVM);

		gpg::AndroidPlatformConfiguration platform_configuration;

		aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();
		std::string mClassPath = "com/myapp/game/MyGame";

		jclass aActivityClass = aEnv->FindClass(mClassPath.c_str());

		std::string pFuncType = "()Lcom/myapp/game/MyGame;";
		jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", "()Lcom/myapp/game/MyGame;");
		jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

		platform_configuration.SetActivity(aActivity);

		auto started_callback = [&](gpg::AuthOperation op){};
		auto finished_callback = [&](gpg::AuthOperation op, gpg::AuthStatus status) {
			isLoggedIn = gpg::IsSuccess(status);
			isFinished = true;
			SDL_CondSignal(authCond);
		};

		isFinished = false;

		game_services = gpg::GameServices::Builder()
			.SetDefaultOnLog(gpg::LogLevel::VERBOSE)
			.SetOnAuthActionStarted(started_callback)
			.SetOnAuthActionFinished(finished_callback)
			.AddOauthScope("https://www.googleapis.com/auth/plus.login")
			.Create(platform_configuration)
			.release();


		if(!isFinished) SDL_CondWait(authCond, authMutex);

		return isLoggedIn;
	}
#endif

bool gpLogin(){

    isFinished = false;

    if(!gpLoggedIn) game_services->StartAuthorizationUI();

    if(!isFinished) SDL_CondWait(authCond, authMutex);

    return isLoggedIn;
}

std::vector<std::string> gpGetFriendIds(){
    if(!gpLoggedIn) gpLogin();

    std::vector<std::string> friendIDs;

    gpg::PlayerManager::FetchListResponse fResponse = game_services->Players().FetchInvitableBlocking();

    for(auto& p : fResponse.data){
        friendIDs.push_back(p.Id());
    }

    return friendIDs;
}

std::string gpGetProfileImage(){
    if(!gpLoggedIn) gpLogin();

    gpg::PlayerManager::FetchSelfResponse usResponse = game_services->Players().FetchSelfBlocking();

    gpg::Player us = usResponse.data;

    if(!us.Valid()) SDL_Log("Invalid player");

    return us.AvatarUrl(gpg::ImageResolution::HI_RES);
}

std::string gpGetName(){
    if(!gpLoggedIn) gpLogin();

    gpg::PlayerManager::FetchSelfResponse usResponse = game_services->Players().FetchSelfBlocking();

    gpg::Player us = usResponse.data;

    if(!us.Valid()) SDL_Log("Invalid player");

    return us.Name();
}


std::string gpGetId(){
    if(!gpLoggedIn) gpLogin();

    gpg::PlayerManager::FetchSelfResponse usResponse = game_services->Players().FetchSelfBlocking();

    gpg::Player us = usResponse.data;

    if(!us.Valid()) SDL_Log("Invalid player");

    return us.Id();
}
