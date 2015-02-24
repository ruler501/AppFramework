#include "GooglePlus.h"

XGooglePlus::XGooglePlus()
{
    // In viewDidLoad, create a platform configuration and
    // bind the ViewController
    NSString *kClientID = @"YOUR_CLIENT_ID";
    gpg::IosPlatformConfiguration config;
    config.SetClientID(std::string([kClientID UTF8String]));
    config.SetOptionalViewControllerForPopups(this);

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
        .Create(config)
        .release();


    if(!isFinished) SDL_CondWait(authCond, authMutex);

    loggedIn = isLoggedIn;
}
