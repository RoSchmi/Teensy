// Teensy41_Midi_Notes_Record_and_Play_On_Audioshield

//https://github.com/FortySevenEffects/arduino_midi_library


// Simple tutorial on how to receive and send MIDI messages.
// Here, when receiving any message on channel 1, the Arduino
// will print messages and play Midi-notes typed on the 
// Yamaha PSR-140 keyboard on the headphones and on the PC
// You can record the sound with e.g. the Audacity App
// Or play the sound on the PC with the C# open source
// Program: 'Repeater Tester' from this Code Project:
// https://www.codeproject.com/Articles/394890/Play-or-Capture-Audio-Sound-Send-and-Receive-as-Mu


// The Midi interface is made with a 6n137 Optocoupler
// https://forum.pjrc.com/threads/66877-Teensy-4-0-serial-optocoupler-6n137s

#include <Arduino.h>
#include <MIDI.h>
//#include "usb_desc.h"
//#include "usb_audio.h"
#include <Audio.h>

#define LED_SIGNAL 4

MIDI_CREATE_DEFAULT_INSTANCE();

bool LedState = false;

// Doesn't matter if AudioOutputUSB is underlined, is injected through build-flags
AudioOutputUSB           usb2;
AudioSynthWaveform waveform1;
AudioMixer4              mixer1;
AudioAmplifier           amp1;
AudioOutputI2S           i2s1;


AudioConnection          patchCord1(waveform1, 0, mixer1, 0);
AudioConnection          patchCord2(mixer1, amp1);
AudioConnection          patchCord3(amp1, 0, i2s1, 0);
AudioConnection          patchCord4(amp1, 0, i2s1, 1);
AudioConnection          patchCord5(amp1, 0, usb2, 0);
AudioConnection          patchCord6(amp1, 0, usb2, 1);
AudioControlSGTL5000     sgtl5000_1;


void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    // Do whatever you want when a note is pressed.

    // Try to keep your callbacks short (no delays ect)
    // otherwise it would slow down the loop() and have a bad impact
    // on real-time performance.
    
    Serial.printf("From eventhandler: Channel: %i pi: %i vel %i\n", (int)channel, (int)pitch, (int)velocity);
    LedState = !LedState;
    digitalWrite(LED_SIGNAL, LedState);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    // Do something when the note is released.
    // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
    
    Serial.print("From eventhandler: Message ended:\n");
    LedState = !LedState;
    digitalWrite(LED_SIGNAL, LedState);  
}

void setup()
{
    pinMode(LED_SIGNAL, OUTPUT);
    
    // Connect the handleNoteOn function to the library,
    // so it is called upon reception of a NoteOn.
    //MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

    // Do the same for NoteOffs
    //MIDI.setHandleNoteOff(handleNoteOff);

    // Launch MIDI and listen to all channels
    MIDI.begin(MIDI_CHANNEL_OMNI);
    
    //start serial with midi baudrate 31250
    Serial1.begin(31250,SERIAL_8N1);

    // Here comes the Audio stuff ***************************
    AudioMemory(20);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.5);

    waveform1.frequency(436);
    waveform1.amplitude(0.0);
    waveform1.begin(WAVEFORM_SINE);
    delay(1000);
}

void loop()
{
  if (MIDI.read())
  {
    int channel = MIDI.getChannel();
    int data1 = MIDI.getData1();
    int data2 = MIDI.getData2();

    if (MIDI.getType() == midi::NoteOn) // 0x90
    {
        Serial.printf("From Loop: Channel: %i Pitch: %i Velocity: %i", channel, data1, data2);
        Serial.println();
        float freq = 440.0 * powf(2.0, (float)(data1 - 69) * 0.08333333);
        waveform1.frequency(freq);
        waveform1.amplitude(0.1);
    }
    else
    {
      if (MIDI.getType() == midi::NoteOff) // 0x80
      {
        Serial.println("From Loop: Note off\n");
        waveform1.amplitude(0.0);
      }
      else
      {
        if (MIDI.getType() != midi::ActiveSensing) // 0xfe
        {
          Serial.printf("%#02x  ", MIDI.getType());
        }
      }
    }
  }
}

