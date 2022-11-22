// Teensy41_USB_Audio_Input
// (uses Audio Shield)
// Design Tool: https://www.pjrc.com/teensy/gui/
//

// In this example sound from Teensy SD-Card is transmitted via USB and played on the PC
// You can record the sound with e.g. the Audacity App
// Important: Set the proper 'build_flags' in platformio.ini 

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>

AudioOutputUSB           usb2;
AudioPlaySdWav           playSdWav1;
AudioOutputI2S           i2s1;
AudioControlSGTL5000     sgtl5000_1;

AudioConnection          patchCord1(playSdWav1, 0, usb2, 0);
AudioConnection          patchCord2(playSdWav1, 1, usb2, 1);
AudioConnection          patchCord3(playSdWav1, 0, i2s1, 0);
AudioConnection          patchCord4(playSdWav1, 1, i2s1, 1);

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

void setup() {
  AudioMemory(30);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.2);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}

void playFile(const char *filename)
{
  playSdWav1.play(filename);
  // A brief delay for the library read WAV info
  delay(5);
  // Simply wait for the file to finish playing.
  while (playSdWav1.isPlaying()) {
  }
}

void loop() { 
  playFile("SDTEST2.WAV");
  delay(500);
  playFile("SDTEST3.WAV");
  delay(500);
  playFile("SDTEST4.WAV");
  delay(1500);
}