// Teensy41_PlaySounds_USB_Keyboard_Test
// The USB Part of this example is from:
// https://github.com/PaulStoffregen/USBHost_t36/blob/master/examples/KeyboardForeward/KeyboardForeward.ino

// Design Tool: https://www.pjrc.com/teensy/gui/
//
// 
// Playing notes on the keyboard / keypad and mix with other waveform
//
// Notes definitions:
// https://dominik-braun.net/frequenztabelle-welche-noten-haben-welche-frequenzen-inkl-pdf-download/

// In this example you can play tones by typing on a connected USB Keyboard
// Typing Esc disables the output
// In the background drum sounds are added
// Play music from SD-Card und add other waveforms by changing the code
#include <Arduino.h>
#include "USBHost_t36.h"
#include "KeyBoardHelper.h"
#include "KeyFrequTable.h"
#include "MixerSettings.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

uint8_t actKeyCode = 0x00;
uint8_t lastKeyCode = 0x00;

float selectedAmp1Gain = 1.0;
float actAmp1Gain = 1.0;


uint32_t lastMillis = 0;

uint32_t DrumIntervals[3] {800, 400, 400};
uint32_t nextInterval = DrumIntervals[0];
int DrumIntervalIndex = 0;
int DrumIntervalsArrayLength = sizeof(DrumIntervals) / sizeof(DrumIntervals[0]); 


bool outputOn = true;

MixerSettings mixerSettings;
GainSetting gainSetting;

AudioSynthWaveformSine sine1;
AudioSynthWaveform waveform1;
AudioSynthSimpleDrum drum1;
AudioSynthSimpleDrum drum2;
AudioSynthSimpleDrum drum3;


AudioPlaySdWav           playSdWav1;   
AudioMixer4              mixer1;
AudioAmplifier           amp1;
AudioOutputI2S           i2s1;
/*
AudioConnection          patchCord1(sine1, 0, mixer1, 0);
AudioConnection        patchCord2(playSdWav1, 0, mixer1, 1);
AudioConnection          patchCord3(playSdWav1, 1, mixer1, 2);
AudioConnection          patchCord4(waveform1, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, amp1);
AudioConnection          patchCord6(amp1, 0, i2s1, 0);
AudioConnection          patchCord7(amp1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;
*/
AudioConnection          patchCord1(sine1, 0, mixer1, 0);
AudioConnection          patchCord2(drum1, 0, mixer1, 1);
AudioConnection          patchCord3(drum2, 0, mixer1, 2);
AudioConnection          patchCord4(drum3, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, amp1);
AudioConnection          patchCord6(amp1, 0, i2s1, 0);
AudioConnection          patchCord7(amp1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// You can have this one only output to the USB type Keyboard and
// not show the keyboard data on Serial...
#define SHOW_KEYBOARD_DATA

USBHost myusb;
USBHub hub1(myusb);
KeyboardController keyboard1(myusb);
USBHIDParser hid1(myusb);
USBHIDParser hid2(myusb);

uint8_t keyboard_modifiers = 0;  // try to keep a reasonable value
#ifdef KEYBOARD_INTERFACE
uint8_t keyboard_last_leds = 0;
#elif !defined(SHOW_KEYBOARD_DATA)
#Warning: "USB type does not have Serial, so turning on SHOW_KEYBOARD_DATA"
#define SHOW_KEYBOARD_DATA
#endif

KeyBoardHelper keyBoardHelper;
KeyFrequTable keyFrequTab;

// forward declarations
void OnPress(int key);
void OnRawPress(uint8_t keycode);
void OnRawRelease(uint8_t keycode);
void OnHIDExtrasPress(uint32_t top, uint16_t key);
void OnHIDExtrasRelease(uint32_t top, uint16_t key);
void ShowUpdatedDeviceListInfo();
void ShowUpdatedDeviceListInfo();
void ShowHIDExtrasPress(uint32_t top, uint16_t key);

void setup()
{
  #ifdef SHOW_KEYBOARD_DATA
    //while (!Serial) ; // wait for Arduino Serial Monitor
    Serial.println("\n\nUSB Host Keyboard forward and Testing");
    Serial.println(sizeof(USBHub), DEC);
  #endif
    myusb.begin();

    // Only needed to display...
  #ifdef SHOW_KEYBOARD_DATA
    keyboard1.attachPress(OnPress);
  #endif

  keyboard1.attachRawPress(OnRawPress);
  keyboard1.attachRawRelease(OnRawRelease);
  keyboard1.attachExtrasPress(OnHIDExtrasPress);
  keyboard1.attachExtrasRelease(OnHIDExtrasRelease);
  
  // Here comes the Audio stuff ***************************
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }

  sine1.frequency(1000);
  sine1.amplitude(0.75);

  waveform1.frequency(436);
  waveform1.amplitude(0.5);
  waveform1.begin(WAVEFORM_SINE);

  drum1.frequency(100);
  //drum1.length(1500);
  drum1.length(600);
  drum1.secondMix(0.0);
  drum1.pitchMod(1.0);

  drum2.frequency(60);
  drum2.length(300);
  drum2.secondMix(0.0);
  drum2.pitchMod(0.55);

  drum3.frequency(60);
  drum3.length(300);
  drum3.secondMix(0.0);
  drum3.pitchMod(0.55);

  gainSetting = mixerSettings.getGainsetting(2);
  mixerSettings.setMixerGainSettings(mixer1, gainSetting);
  
  playSdWav1.play("SDTEST2.WAV");
    delay(10); // wait for library to parse WAV info

  delay(1000);
}

void loop()
{
  myusb.Task();
  ShowUpdatedDeviceListInfo();
  
  // Process keycodes which came from the keyboard
  // actKeyCode is changed in 'OnRawPress' and 'OnRawRelease'
  if (actKeyCode != lastKeyCode)
  {
    sine1.frequency(keyFrequTab.getFrequ(actKeyCode));
    sine1.amplitude(0.75);
    
  }
  if ((millis() - lastMillis) > nextInterval )
  {
    lastMillis = millis();

    switch (DrumIntervalIndex)
    {
      case 0: drum1.noteOn(); Serial.println("Drum1"); break;
      case 1: drum2.noteOn(); Serial.println("Drum2"); break;
      case 2: drum3.noteOn(); Serial.println("Drum3");break;
    }
    int nextIndex = DrumIntervalIndex + 1;
    DrumIntervalIndex = DrumIntervalIndex == DrumIntervalsArrayLength - 1 ?  0 : nextIndex;
    nextInterval = DrumIntervals[DrumIntervalIndex];
   
  }
  if (actKeyCode == 0x00)
  {
      sine1.amplitude(0);   
  }
  
  // Esc or minus on the keypad toggle 'outputOn'
  // see code code in 'OnRawRelease'
  float amp1Gain = outputOn ? selectedAmp1Gain : 0.0;
  if (actAmp1Gain != amp1Gain)
  {
    amp1.gain(amp1Gain);
    actAmp1Gain = amp1Gain;
  }
}

void OnRawPress(uint8_t keycode) {
#ifdef KEYBOARD_INTERFACE
  if (keyboard_leds != keyboard_last_leds) {
    //Serial.printf("New LEDS: %x\n", keyboard_leds);
    keyboard_last_leds = keyboard_leds;
    keyboard1.LEDS(keyboard_leds);
  }
  if (keycode >= 103 && keycode < 111) {
    // one of the modifier keys was pressed, so lets turn it
    // on global..
    uint8_t keybit = 1 << (keycode - 103);
    keyboard_modifiers |= keybit;
    Keyboard.set_modifier(keyboard_modifiers);
  } else {
    if (keyboard1.getModifiers() != keyboard_modifiers) {
      #ifdef SHOW_KEYBOARD_DATA
      Serial.printf("Mods mismatch: %x != %x\n", keyboard_modifiers, keyboard1.getModifiers());
      #endif
      keyboard_modifiers = keyboard1.getModifiers();
      Keyboard.set_modifier(keyboard_modifiers);
    }
    Keyboard.press(0XF000 | keycode);
  }
#endif
#ifdef SHOW_KEYBOARD_DATA
  Serial.print("OnRawPress keycode: ");
  Serial.print(keycode, HEX);
  Serial.print(" Modifiers: ");
  Serial.println(keyboard_modifiers, HEX);
#endif

lastKeyCode = actKeyCode;
actKeyCode = keycode;

}
void OnRawRelease(uint8_t keycode) {
#ifdef KEYBOARD_INTERFACE
  if (keycode >= 103 && keycode < 111) {
    // one of the modifier keys was pressed, so lets turn it
    // on global..
    uint8_t keybit = 1 << (keycode - 103);
    keyboard_modifiers &= ~keybit;
    Keyboard.set_modifier(keyboard_modifiers);
  } else {
    Keyboard.release(0XF000 | keycode);
  }
#endif
#ifdef SHOW_KEYBOARD_DATA
  Serial.print("OnRawRelease keycode: ");
  Serial.print(keycode, HEX);
  Serial.print(" Modifiers: ");
  Serial.println(keyboard1.getModifiers(), HEX);
#endif

  actKeyCode = keycode == actKeyCode ? 0.0 : keycode;
  
  if ((keycode == 0x29) | (keycode == 0x56))  // Esc or ninus on keypad toggle mute
    {
      outputOn = !outputOn;
    } 
}

void OnHIDExtrasPress(uint32_t top, uint16_t key)
{
#ifdef KEYBOARD_INTERFACE
  if (top == 0xc0000) {
    Keyboard.press(0XE400 | key);
#ifndef KEYMEDIA_INTERFACE
#error "KEYMEDIA_INTERFACE is Not defined"
#endif
  }
#endif
#ifdef SHOW_KEYBOARD_DATA
  ShowHIDExtrasPress(top, key);
#endif
}

void OnHIDExtrasRelease(uint32_t top, uint16_t key)
{
#ifdef KEYBOARD_INTERFACE
  if (top == 0xc0000) {
    Keyboard.release(0XE400 | key);
  }
#endif
#ifdef SHOW_KEYBOARD_DATA
  Serial.print("HID (");
  Serial.print(top, HEX);
  Serial.print(") key release:");
  Serial.println(key, HEX);
#endif
}

//=============================================================
// Device and Keyboard Output To Serial objects...
//=============================================================
#ifdef SHOW_KEYBOARD_DATA
USBDriver *drivers[] = {&hub1, &hid1, &hid2};
#define CNT_DEVICES (sizeof(drivers)/sizeof(drivers[0]))
const char * driver_names[CNT_DEVICES] = {"Hub1", "HID1" , "HID2"};
bool driver_active[CNT_DEVICES] = {false, false, false};

// Lets also look at HID Input devices
USBHIDInput *hiddrivers[] = { &keyboard1 };
#define CNT_HIDDEVICES (sizeof(hiddrivers) / sizeof(hiddrivers[0]))
const char *hid_driver_names[CNT_DEVICES] = { "KB" };
bool hid_driver_active[CNT_DEVICES] = { false };

#endif

void ShowUpdatedDeviceListInfo()
{
#ifdef SHOW_KEYBOARD_DATA
  for (uint8_t i = 0; i < CNT_DEVICES; i++) {
    if (*drivers[i] != driver_active[i]) {
      if (driver_active[i]) {
        Serial.printf("*** Device %s - disconnected ***\n", driver_names[i]);
        driver_active[i] = false;
      } else {
        Serial.printf("*** Device %s %x:%x - connected ***\n", driver_names[i], drivers[i]->idVendor(), drivers[i]->idProduct());
        driver_active[i] = true;

        const uint8_t *psz = drivers[i]->manufacturer();
        if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
        psz = drivers[i]->product();
        if (psz && *psz) Serial.printf("  product: %s\n", psz);
        psz = drivers[i]->serialNumber();
        if (psz && *psz) Serial.printf("  Serial: %s\n", psz);

      }
    }
  }
  for (uint8_t i = 0; i < CNT_HIDDEVICES; i++) {
    if (*hiddrivers[i] != hid_driver_active[i]) {
      if (hid_driver_active[i]) {
        Serial.printf("*** HID Device %s - disconnected ***\n", hid_driver_names[i]);
        hid_driver_active[i] = false;
      } else {
        Serial.printf("*** HID Device %s %x:%x - connected ***\n", hid_driver_names[i], hiddrivers[i]->idVendor(), hiddrivers[i]->idProduct());
        hid_driver_active[i] = true;

        const uint8_t *psz = hiddrivers[i]->manufacturer();
        if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
        psz = hiddrivers[i]->product();
        if (psz && *psz) Serial.printf("  product: %s\n", psz);
        psz = hiddrivers[i]->serialNumber();
        if (psz && *psz) Serial.printf("  Serial: %s\n", psz);
        // Note: with some keyboards there is an issue that they may not output in a format understand
        // either as in boot format or in a HID format that is recognized.  In that case you
        // can try forcing the keyboard into boot mode.
        if (hiddrivers[i] == &keyboard1) {
          // example Gigabyte uses N key rollover which should now work, but...
          #if 0
          if (keyboard1.idVendor() == 0x04D9) {
            Serial.println("Gigabyte vendor: force boot protocol");
            // Gigabyte keyboard
            keyboard1.forceBootProtocol();
          }
          #endif
        }
      }
    }
  }
#endif
}

#ifdef SHOW_KEYBOARD_DATA
void OnPress(int key)
{
  keyBoardHelper.PrintKey(key);  
  
  Serial.print("'  ");
  Serial.print(key);
  Serial.print(" MOD: ");
  Serial.print(keyboard1.getModifiers(), HEX);
  Serial.print(" OEM: ");
  Serial.print(keyboard1.getOemKey(), HEX);
  Serial.print(" LEDS: ");
  Serial.println(keyboard1.LEDS(), HEX);
  //Serial.print("key ");
  //Serial.print((char)keyboard1.getKey());
  //Serial.print("  ");
  //Serial.print((char)keyboard2.getKey());
  //Serial.println();
  
}
#endif

void ShowHIDExtrasPress(uint32_t top, uint16_t key)
{
#ifdef SHOW_KEYBOARD_DATA
  keyBoardHelper.ShowHIDExtrasPress(top, key);
#endif
}