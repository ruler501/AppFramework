# Bluetooth

## How to Use This

    XBluetooth bluetooth;
    if(bluetooth.initiate())//Only needs to be called once
        if(bluetooth.findAndConnect("Device Name", "UUID in Java Format"))
            ....
    
Then you can call `sendBytes(std::vector<uint8_t>{0,1,2})` or `receiveBytes(3)`

## Implmentation

### Android
Calls out to JNI for every call and as such is expensive performance wise. It also currently doesn't thread so may cause delays.

###iOS
Yet to be implemented, I don't have a device so I can't test any code I develop
