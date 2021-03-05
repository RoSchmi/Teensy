// Example for Teensy 4.1, setting the RTC from a NTP request

// Cave !!!!!
// There are problems concerning  the Time.h library and C time.h library (which is additionally needed) on Teensy 4.1 
// To reach that the C time.h is included go to:
// C:\Users\User\.platformio\packages\framework-arduinoteensy\libraries\Time
// and rename Time.h to Time.h_

#include <Arduino.h>
#include "DateTime.h"
#include "SysTime.h"
#include "defines.h"

#include <NTPClient_Generic.h>

#include <NativeEthernet.h>

// Class to access RTC
static SysTime sysTime;

// A UDP instance to let us send and receive packets over UDP
EthernetUDP ntpUDP;

#define TIME_ZONE_OFFSET_HRS            (1)

NTPClient timeClient(ntpUDP);

unsigned long utcTime;      // Seconds since 1. Jan. 1970

DateTime dateTimeUTCNow;    // Seconds since 2000-01-01 08:00:00

void setup(){
  Serial.begin(115200);
  while (!Serial);
  Serial.print("\nStart Ethernet_NTPClient_Basic on " + String(BOARD_NAME));
  Serial.println(" with " + String(SHIELD_TYPE));
  
  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  uint16_t index = millis() % NUMBER_OF_MAC;
  // Use Static IP
  //Ethernet.begin(mac[index], ip);
  Ethernet.begin(mac[index]);

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

  timeClient.begin();
  // default 60000 => 60s. Set to once per hour
  timeClient.setUpdateInterval(SECS_IN_HR);
  Serial.println("Using NTP Server " + timeClient.getPoolServerName());
  
  timeClient.update();
  uint32_t counter = 0;
  uint32_t maxCounter = 10;
  
  while(!timeClient.updated() &&  counter++ <= maxCounter)
  {
    Serial.println("NTP FAILED: Trying again");
    delay(1000);
    timeClient.update();
  }

  if (counter >= maxCounter)
  {
    while(true)
    {
      delay(500); //Wait for ever
    }
  }
  
  Serial.println("\r\n********UPDATED********");
    
  Serial.println("UTC : " + timeClient.getFormattedUTCTime());
  Serial.println("UTC : " + timeClient.getFormattedUTCDateTime());
  Serial.println("LOC : " + timeClient.getFormattedTime());
  Serial.println("LOC : " + timeClient.getFormattedDateTime());
  Serial.println("UTC EPOCH : " + String(timeClient.getUTCEpochTime()));
  Serial.println("LOC EPOCH : " + String(timeClient.getEpochTime()));

  utcTime = timeClient.getUTCEpochTime();  // Seconds since 1. Jan. 1970

  sysTime.begin(utcTime + SECONDS_FROM_1970_TO_2000);

  dateTimeUTCNow = sysTime.getTime();
  
  Serial.printf("%s %i %i %i %i %i", (char *)"UTC-Time is", dateTimeUTCNow.year(), 
                                        dateTimeUTCNow.month() , dateTimeUTCNow.day(),
                                        dateTimeUTCNow.hour() , dateTimeUTCNow.minute());
  Serial.println("");
}

void loop() 
{
  delay(5000);
  dateTimeUTCNow = sysTime.getTime();
  Serial.printf("%s %i %i %i %i:%i:%i", (char *)"UTC-Time is", dateTimeUTCNow.year(), 
                                        dateTimeUTCNow.month() , dateTimeUTCNow.day(),
                                        dateTimeUTCNow.hour() , dateTimeUTCNow.minute(), dateTimeUTCNow.second() );
  Serial.println("");
}