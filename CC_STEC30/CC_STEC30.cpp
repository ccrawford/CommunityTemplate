#include "CC_STEC30.h"
#include "allocateMem.h"
#include "commandmessenger.h"

/* **********************************************************************************
    This is just the basic code to set up your custom device.
    Change/add your code as needed.
********************************************************************************** */

// Not passing in any pins. They will all be hardcoded for the device.
CC_STEC30::CC_STEC30()
{
}

void CC_STEC30::begin()
{
}

void CC_STEC30::attach(uint16_t Pin3, char *init)
{
    _pin3 = Pin3;
}

void CC_STEC30::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void CC_STEC30::set(int16_t messageID, char *setPoint)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do.
        Important Remark!
        MessageID == -1 will be send from the connector when Mobiflight is closed
        Put in your code to shut down your custom device (e.g. clear a display)
        MessageID == -2 will be send from the connector when PowerSavingMode is entered
        Put in your code to enter this mode (e.g. clear a display)

    ********************************************************************************** */
    int32_t  data = atoi(setPoint);
    uint16_t output;

    // do something according your messageID
    switch (messageID) {
    case -1:
        // tbd., get's called when Mobiflight shuts down
    case -2:
        // tbd., get's called when PowerSavingMode is entered
    case 0:
        output = (uint16_t)data;
        data   = output;
        break;
    case 1:
        /* code */
        break;
    case 2:
        /* code */
        break;
    default:
        break;
    }
}

void CC_STEC30::update()
{
    // Do something which is required regulary
}
