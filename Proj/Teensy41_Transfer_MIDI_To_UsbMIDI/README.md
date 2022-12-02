### Teensy41_Tranfer_MIDI_To_UsbMidi

Teensy 4.1 receives MIDI Notes from up to 3 MIDI-Devices (e.g. MIDI-Keyboard) through
Serial ports via 6N137/6N138 optocouplers and transfers the MIDI-Notes via the
USB-Slave port to a Windows PC where the notes can be captured to be recorded
in a .midi file. The reverse direction of the code was not tested here.
The notes can be captured on the PC side with e.g. the commercially avaIlable
Program 'MixPad Multitrak Recording Software' or my C# App 'RoSchmiDryWetMidiUtility'
(coming soon), which is based on the https://github.com/melanchall/drywetmidi library.
   
The code for the Teensy is an adaption of the this example:
https://github.com/PaulStoffregen/Teensyduino_Examples/blob/master/USB_MIDI/Interface_3x3/Interface_3x3.ino












