#include "VolumeController.h"

VolumeController_class::VolumeController_class(int slot)
{
    base=IO_SLOT(slot);
}

void VolumeController_class::set(int input, int left, int right)
{
    left&=0xff;
    right&=0xff;
    input&=7;
    REGISTER( base, input ) = left;
    REGISTER( base, input + 32 ) = right;
}


VolumeController_class VolumeController;
