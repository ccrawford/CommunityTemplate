#include "CC_STEC30.h"
#include "allocateMem.h"
#include "commandmessenger.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "dial_image.h"
#include "plane_image.h"
#include "ball_image.h"
#include "LED_Images.h"

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
// Sprites used in display
TFT_eSprite mainSpr = TFT_eSprite(&tft); // Main sprite. Full screen.

// The plane and the ball need to be sprites so they can be rotated and masked.
TFT_eSprite planeSpr = TFT_eSprite(&tft); // Plane sprite
TFT_eSprite ballSpr = TFT_eSprite(&tft);  // Turn coordinator ball sprite


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
      pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);

    tft.begin();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);

    mainSpr.createSprite(320, 300);
    mainSpr.setSwapBytes(true);
    mainSpr.pushImage(0, 0, dialWidth, dialHeight, dial);
    mainSpr.pushSprite(0, 0);

    mainSpr.setPivot(160, 150);                       // Set the pivot point for the rotation of the background

  // Create a sprite to hold the jpeg (or part of it)
  // CAC set impage depth.
  // planeSpr.setColorDepth(16);
  planeSpr.createSprite(planeOutlineWidth, planeOutlineHeight);
  planeSpr.setPivot(83, 22); // Determined in paint program. Plane rotates around the center of the fuselage.
  planeSpr.setSwapBytes(true);
  planeSpr.pushImage(0, 0, planeOutlineWidth, planeOutlineHeight, planeOutline);

  ballSpr.createSprite(ballWidth, ballHeight);
  ballSpr.pushImage(0, 0, ballWidth, ballHeight, ball_image);

}

void CC_STEC30::attach()
{
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
    // int32_t  data = atoi(setPoint);
    // uint16_t output;


    // do something according your messageID
    switch (messageID) {
    case -1:
        // tbd., get's called when Mobiflight shuts down
        break;
    case -2:
        break;
        // tbd., get's called when PowerSavingMode is entered
    case 0:
        setTurnCoordNeedle(atof(setPoint));
        break;
    case 1:
        setInclinometerBall(atof(setPoint));
        break;
    case 2:
        setApTrimUpLight(atoi(setPoint) > 0);
        break;
    case 3:
        setApTrimDownLight(atoi(setPoint) > 0);
        break;
    case 4:
        setApTrkLoLight(atoi(setPoint) > 0);
        break;
    case 5:
        setApTrkHiLight(atoi(setPoint) > 0);
        break;
    case 6:
        setApStLight(atoi(setPoint) > 0);
        break;
    case 7:
        setApRdyLight(atoi(setPoint) > 0);
        break;
    case 8:
        setApHdLight(atoi(setPoint) > 0);
        break;
    case 9:
        setApAltLight(atoi(setPoint) > 0);
        break;
    case 10:
        setLowVoltLight(atoi(setPoint) > 0);
        break;
    
    default:
        break;
    }
}

void CC_STEC30::update()
{
    setInclinometerBall(inclinometerBall);
    setTurnCoordNeedle(turnCoorNeedle);

    setApAltLight(altLedState);
    setApHdLight(hdLedState);
    setApRdyLight(rdyLedState);
    setApStLight(stLedState);
    setApTrkLoLight(trkLoLedState);
    setApTrkHiLight(trkHiLedState);
    setApTrimUpLight(upLedState);
    setApTrimDownLight(downLedState);
    setLowVoltLight(lowVoltLedState);

    // "Clear" the screen by putting up a fresh background
    mainSpr.pushImage(0, 0, dialWidth, dialHeight, dial); 
    displayBall();
    displayLeds();

    // Do the plane last. It's on top of all the others.
    displayTurnCoordNeedle();

    mainSpr.pushSprite(0, 0); // Push the main sprite to the screen
}
void CC_STEC30::displayLeds()
{
  if (stLedState) mainSpr.pushImage(STDotX, STDotY, APDot_width, APDot_height, APDot);
  if (hdLedState) mainSpr.pushImage(HDDotX, HDDotY, APDot_width, APDot_height, APDot);
  if (trkLoLedState) mainSpr.pushImage(TrkLoDotX, TrkLoDotY, APDot_width, APDot_height, APDot);
  if (trkHiLedState) mainSpr.pushImage(TrkHiDotX, TrkHiDotY, APDot_width, APDot_height, APDot);

  if (altLedState) mainSpr.pushImage(AltDot_x, AltDot_y, AltDot_width, AltDot_height, AltDot);

  if (upLedState) mainSpr.pushImage(UpDot_x, UpDot_y, UpDot_width, UpDot_height, UpDot);
  if (downLedState) mainSpr.pushImage(DownDot_x, DownDot_y, DownDot_width, DownDot_height, DownDot);

  if (rdyLedState) mainSpr.pushImage(ReadyDot_x, ReadyDot_y, ReadyDot_width, ReadyDot_height, ReadyDot);

  if (lowVoltLedState) mainSpr.pushImage(LowVoltFlag_x, LowVoltFlag_y, LowVoltFlag_width, LowVoltFlag_height, LowVoltFlag);

  return;
}

// number from 0 to 100, with 50 being centered.
void CC_STEC30::setTurnCoordNeedle(double percent)
{
  turnCoorNeedle = percent;
}

void CC_STEC30::setInclinometerBall(double percent)
{
  inclinometerBall = percent;
}

void CC_STEC30::displayBall()
{ 
  double angle = inclinometerBall*50.0;
  ballSpr.pushToSprite(&mainSpr, mainSpr.width() / 2 + angle - 13, mainSpr.height() - ballSpr.height() - round((abs(angle) / 5)) - 22, TFT_WHITE);
}

void CC_STEC30::displayTurnCoordNeedle()
{
  double angle = (turnCoorNeedle - 50) * 0.6;

  planeSpr.pushRotated( &mainSpr, (int16_t)angle, TFT_WHITE); // Push the plane sprite on the background

}

void CC_STEC30::setApTrimUpLight(bool state)
{
  upLedState = state;
}
void CC_STEC30::setApTrimDownLight(bool state)
{
  downLedState = state;
}
void CC_STEC30::setApTrkLoLight(bool state)
{
  trkLoLedState = state;
}
void CC_STEC30::setApTrkHiLight(bool state)
{
  trkHiLedState = state;
}
void CC_STEC30::setApStLight(bool state)
{
  stLedState = state;
}
void CC_STEC30::setApRdyLight(bool state)
{
  rdyLedState = state;
}
void CC_STEC30::setApHdLight(bool state)
{
  hdLedState = state;
}
void CC_STEC30::setApAltLight(bool state)
{
  altLedState = state;
}
void CC_STEC30::setLowVoltLight(bool state)
{
  lowVoltLedState = state;
}