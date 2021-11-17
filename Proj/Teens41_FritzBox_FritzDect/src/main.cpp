// Program Teensy_FritzBox_FritzDect. Copyright RoSchmi 2021, License MIT
// https://github.com/RoSchmi/Teensy/tree/master/Proj/Teens41_FritzBox_FritzDect

// This is an adaption/modification of:
// https://github.com/planetk/ArduinoFritzApi 

// For the MD5 hash calculation the used way was taken from
// https://github.com/schuppeste/Arduino-Fritzbox-Login

// This application runs on a Teensy 4.1 
// It is used to retrieve power, energy and temperature measurements
// and switch the output of a Fritz!Dect switchable
// Power Socket via a FritzBox WLAN Router/DECT phone.
// The FritzBox family is a popular WLAN Router/DECT-Phone combination in 
// Germany
// The communication between Teensy and FritzBox works via Ethernet
// and http and https transmission is supported
// The communication between FritzBox and the Fritz!Dect power socket
// works via DECT radiotransmission.
    

#include <Arduino.h>
#include <SPI.h>
#include "defines.h"
#include "Entropy.h"
#include "config.h"
#include "config_secret.h"
// SSL Certificates for server verfication are in this file
// check in 'trust_anchors.h' how to get the certificate of fritz.box
#include "trust_anchors.h"
#include "EthernetWebServer_SSL.h"
#include "RsArduinoFritzApi.h"

//https://github.com/tzikis/ArduinoMD5/
#include <MD5.h>



static EthernetClient  client;

static EthernetSSLClient sslClient(client, TAs, (size_t)TAs_NUM);


#if TRANSPORT_PROTOCOL == 1    
    static EthernetHttpClient  httpClient(sslClient, (char *)FRITZ_IP_ADDRESS, (TRANSPORT_PROTOCOL == 0) ? 80 : 443);
    Protocol protocol = Protocol::useHttps;
  #else
    static EthernetHttpClient  httpClient(client, (char *)FRITZ_IP_ADDRESS, (TRANSPORT_PROTOCOL == 0) ? 80 : 443);
    Protocol protocol = Protocol::useHttp;
  #endif

  FritzApi fritz((char *)FRITZ_USER, (char *)FRITZ_PASSWORD, FRITZ_IP_ADDRESS, protocol, &client, &sslClient, &httpClient);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  
  Serial.println(F("Starting"));
  delay(2000);
  
  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac

  //uint16_t index = millis() % NUMBER_OF_MAC;
   uint16_t index = 0;

  //Ethernet.setSocketNum(4);
  //Ethernet.setSocketSize(1024 * 4);

  // Set DNS sever to your choice (if wanted)
  /*
  uint8_t theDNS_Server_Ip[4] {8,8,8,8};
  Ethernet.setDnsServerIP(theDNS_Server_Ip);
  */

  #if USE_STATIC_IP
  // Set ip address of your choice
  IPAddress ip = {192, 168, 1, 106};
  Ethernet.begin(mac[index], ip);
  #else
  Serial.println(F("Starting DHCP"));
  if (Ethernet.begin(mac[index]) != 1)
  {
    while (true) 
    {
      Serial.println(F("Failed to configure Ethernet using DHCP"));
      delay(5000); // do nothing, no point running without Ethernet hardware
    }
  }
  #endif

   // Just info to know how to connect correctly
  Serial.println(F("========================="));
  Serial.println(F("Currently Used SPI pinout:"));
  Serial.print(F("MOSI:"));
  Serial.println(MOSI);
  Serial.print(F("MISO:"));
  Serial.println(MISO);
  Serial.print(F("SCK:"));
  Serial.println(SCK);
  Serial.print(F("SS:"));
  Serial.println(SS);

  Serial.println(F("========================="));

  Serial.print(F("Using mac index = "));
  Serial.println(index);

  // you're connected now, so print out the data
  Serial.print(F("You're connected to the network, IP = "));
  Serial.println(Ethernet.localIP());
  
  if (fritz.init())
  {
    Serial.println("Initialization for FritzBox is done");
  }
  else
  {
    Serial.println("Initialization for FritzBox failed");
    while (true)
    {
      delay(200);
    }    
  }
  
  delay(1000);

  String actualSID = fritz.testSID();
  Serial.print("Actual SID is: ");
  Serial.println(actualSID);
}

void loop() {

  String switchname = fritz.getSwitchName(FRITZ_DEVICE_AIN_01); 
  Serial.printf("%s%s", F("Name of device is: "), switchname.c_str());

  bool present_01 = fritz.getSwitchPresent(FRITZ_DEVICE_AIN_01);
  Serial.printf("%s%s%s%s", "Device is ", present_01 ? "" : "not ", F("present"), "\r\n");
  
  //Serial.println("Switch is now toogled");
  //bool newState_01 = fritz.setSwitchToggle(FRITZ_DEVICE_AIN_01);

  bool state_01 = fritz.getSwitchState(FRITZ_DEVICE_AIN_01);
  Serial.printf("%s%s%s", "State is ", state_01 ? "on" : "off", "\r\n");
  
  double actPower =  fritz.getSwitchPower(FRITZ_DEVICE_AIN_01);
  Serial.print(F("Power is: "));
  Serial.println(actPower);

  double actEnergy =  fritz.getSwitchEnergy(FRITZ_DEVICE_AIN_01);
  Serial.print(F("Energy is: "));
  Serial.println(actEnergy);

  double actTemperature = fritz.getTemperature(FRITZ_DEVICE_AIN_01);
  Serial.print(F("Temperature is: "));
  Serial.println(actTemperature);

  if (state_01)
  {
     Serial.println(F("Switch is now set to off (function inactivated)"));
     // Uncomment to activate
     // bool newState_01 = fritz.setSwitchOff(FRITZ_DEVICE_AIN_01);
  }
  else
  {
     Serial.println(F("Switch is now set to on (function inactivated)"));
     // Uncomment to activate
     // bool newState_01 = fritz.setSwitchOn(FRITZ_DEVICE_AIN_01);
  }
  
  delay( 30 * 60 * 1000);
}