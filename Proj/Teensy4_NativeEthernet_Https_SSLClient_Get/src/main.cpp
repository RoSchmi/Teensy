

// Teensy4_NativeEthernet_Https_SSLClient_Get

// Teensy 4.1 downloads a site SSL secures using 'NativeEthernet'- and 'EthernetHttpClient_SSL' library

// https://github.com/vjmuzik/NativeEthernet
// https://github.com/vjmuzik/FNET
// https://github.com/khoih-prog/EthernetWebServer_SSL

// Make sure that the latest library version of NativeEthernet is included (see platformio.ini)
// The version actually (2021/03/14) included in .platformio/packages/framework-arduinoteensy
// doesnt't work

// Cave:
// Replace the Stream.h file in  C:/Users/thisUser/.platformio/packages/framework-arduinoteensy/cores/teensy4/Stream.h
// with the custom version found in lib/RoSchmi/Stream 


#include <Arduino.h>
#include "defines.h"

// SSL Certificates for server verfication are in this file
#include "trust_anchors.h"

#include <NativeEthernet.h>
#include "EthernetHttpClient_SSL.h"
#include "EthernetWebServer_SSL.h"

#include "functional-vlpp.h"

// initialize the library instance:
EthernetClient client;

//EthernetSSLClient sslClient(client, TAs, (size_t)TAs_NUM, 2, EthernetSSLClient::SSL_DUMP);   // Define Log Level
EthernetSSLClient sslClient(client, TAs, (size_t)TAs_NUM);

//char server[] = "pjrc.com";  // also change the Host line in httpRequest()
const char server[] = "www.google.com";

//const char kPath[] = "/about/contact.html";
const char kPath[] = "/";                 // for google



EthernetHttpClient httpClient(sslClient, server, 443);
//EthernetHttpClient httpClient(client, server, 80);

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10*1000;  // delay between updates, in milliseconds

//Forward declarations
void httpRequest();


void setup() { 
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  uint16_t index = millis() % NUMBER_OF_MAC;
  
  Serial.println(F("Starting DHCP"));

  
  
  //-https://forum.pjrc.com/threads/63373-Teensy-4-1-ethernet-webserver-problem 
  //-https://forum.pjrc.com/threads/60857-T4-1-Ethernet-Library
  //-https://github.com/khoih-prog/EthernetWebServer_SSL#read-buffer-overflow



  //Ethernet.setStackHeap(1024 * 128);
  //Ethernet.setSocketSize(1024 * 6);
  //Ethernet.setSocketNum(2);

  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac[index]) != 1) 
  {
    while (true) 
    {
      Serial.println("Failed to configure Ethernet using DHCP");
      delay(5000); // do nothing, no point running without Ethernet hardware
    }
  }
  // you're connected now, so print out the data
  Serial.print(F("You're connected to the network, IP = "));
  Serial.println(Ethernet.localIP());
  delay(1000);
  
}

void loop()
{
  int err = 0;
  
  err = httpClient.get(kPath);

  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = httpClient.responseStatusCode();

    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = httpClient.skipResponseHeaders();

      if (err >= 0)
      {
        int bodyLen = httpClient.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");

        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();

        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (httpClient.connected() || httpClient.available()) &&
                ((millis() - timeoutStart) < 7000) )
        {
          if (httpClient.available())
          {
            Serial.print((char) httpClient.read());

            bodyLen--;

            // We read something, reset the timeout counter
            timeoutStart = millis();
          }
          else
          {
            // We haven't got any data, so let's pause to allow some to
            // arrive       
            delay(200);
          }
        }        
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else
    {
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  // And just stop, now that we've tried a download
  Serial.print("Stopping httpclient");
  //client.close();
  httpClient.stop();

  delay(5000);
  //while (1);
}


