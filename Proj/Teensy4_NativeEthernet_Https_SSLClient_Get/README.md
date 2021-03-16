# Teensy4_NativeEthernet_Https_SSLClient_Get

Teensy 4.1 downloads a site SSL secures using 'NativeEthernet'- and 'EthernetHttpClient_SSL' library

// https://github.com/vjmuzik/NativeEthernet
// https://github.com/vjmuzik/FNET
// https://github.com/khoih-prog/EthernetWebServer_SSL

Make sure that the latest library version of NativeEthernet is included (see platformio.ini)
The version actually (2021/03/14) included in .platformio/packages/framework-arduinoteensy
doesnt't work

Cave:
Replace the Stream.h file in  C:/Users/thisUser/.platformio/packages/framework-arduinoteensy/cores/teensy4/Stream.h
with the custom version found in lib/RoSchmi/Stream 


