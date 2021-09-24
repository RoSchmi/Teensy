#include <Arduino.h>
#include <SPI.h>
#include "defines.h"
#include "Entropy.h"

#include "config.h"
#include "config_secret.h"
// SSL Certificates for server verfication are in this file
#include "trust_anchors.h"
#include "EthernetWebServer_SSL.h"
#include "RsArduinoFritzApi.h"
//#include <MD5Builder.h>

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

  //FritzApi fritz((char *)FRITZ_USER, (char *)FRITZ_PASSWORD, protocol, &httpClient );
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
  }
  
  delay(3000);

  bool isValid_SID = fritz.testSID();

  //bool state_01 = fritz.getSwitchState(FRITZ_DEVICE_AIN_01);

}

void loop() {

  
  
  bool state_01 = fritz.getSwitchState(FRITZ_DEVICE_AIN_01);

  if(state_01)
  {
  Serial.println("Switch is on");
  }
  else
  {
    Serial.println("Switch is off");
  }
  
  delay(3000);

  // put your main code here, to run repeatedly:
}