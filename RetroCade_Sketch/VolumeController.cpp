#include "VolumeController.h"

VolumeController_class::VolumeController_class(int slot)
{
    base=IO_SLOT(slot);
}

void VolumeController_class::set(int input, int left, int right)
{
    left&=0xffff;
    right&=0xffff;
    input&=7;
    REGISTER( base, input ) = left;
    REGISTER( base, input + 8 ) = right;
}


VolumeController_class VolumeController;
