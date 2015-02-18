#ifndef _XNOTIFICATION_H_
#define _XNOTIFICATION_H_

#include <string>

class XNotification{
private:
    std::string iconName, title, text;
    int id;
    bool dispatched;

public:
    static int lastID;

    XNotification(std::string _iconName, std::string _title, std::string _text, bool _dispatch);

    bool dispatch();
    bool cancel();

    ~XNotification();
};

#endif // _XNOTIFICATION_H_
