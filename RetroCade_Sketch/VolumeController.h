#include <Arduino.h>

class VolumeController_class
{
public:
    VolumeController_class(int slot=12);

    void set(int input, int left, int right);

private:
    unsigned base;
};

extern VolumeController_class VolumeController;

