#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include <vector>
#include <string>
#include "SDL.h"

class XBluetooth{
private:
    bool initiated, connected;

public:
    XBluetooth() : initiated(false), connected(false) {};

    bool initiate();

    bool findDevice(std::string name);

    bool connectDevice(std::string uuid);

    bool findAndConnect(std::string name, std::string uuid);

    std::vector<uint8_t> receiveBytes(size_t amount);

    bool sendBytes(std::vector<uint8_t> bytes);
};

#endif // _BLUETOOTH_H_
