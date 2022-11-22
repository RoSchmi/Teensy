// Teensy41_USB_Audio_Input
// (uses Audio Shield)
// Design Tool: https://www.pjrc.com/teensy/gui/
//

// In this example sound from the PC is transmitted via USB and played on the Teensy
// Important: Set the proper 'build_flags' in platformio.ini 

#include <Arduino.h>
//#include "usb_desc.h"
//#include "usb_audio.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>

AudioInputUSB            usb1;
AudioOutputI2S           i2s1;
AudioControlSGTL5000     sgtl5000_1;

AudioConnection          patchCord1(usb1, 0, i2s1, 0);
AudioConnection          patchCord2(usb1, 1, i2s1, 1);

void setup() {
  AudioMemory(30);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
}

void loop() {
  // put your main code here, to run repeatedly:
}