#include <Arduino.h>
#include <MIDI.h>


//https://github.com/FortySevenEffects/arduino_midi_library

// Teensy41_Midi_Get_Notes_from_Yamaha_PSR140_Keyboard

// Simple tutorial on how to receive and send MIDI messages.
// Here, when receiving any message on channel 1, the Arduino
// will blink a led and print messages in eventhandler and loop

// The Midi interface is made with a 6n137 Optocoupler
// https://forum.pjrc.com/threads/66877-Teensy-4-0-serial-optocoupler-6n137s

#define LED_SIGNAL 4

MIDI_CREATE_DEFAULT_INSTANCE();

bool LedState = false;

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
    MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

    // Do the same for NoteOffs
    MIDI.setHandleNoteOff(handleNoteOff);

    // Launch MIDI and listen to all channels
    MIDI.begin(MIDI_CHANNEL_OMNI);
    
    //start serial with midi baudrate 31250
    Serial1.begin(31250,SERIAL_8N1);
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
    }
    else
    {
      if (MIDI.getType() == midi::NoteOff) // 0x80
      {
        Serial.println("From Loop: Note off\n");
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

