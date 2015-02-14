#include "XGooglePlus.h"

XGooglePlus::XGooglePlus()
: authCond(SDL_CreateCond()), authMutex(SDL_CreateMutex()), isLoggedIn(false),
  isFinished(false)
{

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

    loggedIn = isLoggedIn;
}
