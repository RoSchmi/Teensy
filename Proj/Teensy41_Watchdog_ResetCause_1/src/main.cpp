// Example illustrating watchdog WDT1 and retrieving last 'Reset Cause'

//-https://forum.pjrc.com/threads/59257-WDT_T4-Watchdog-Library-for-Teensy-4
//-https://github.com/tonton81/WDT_T4

#include <Arduino.h>
#include "Watchdog_t4.h"
WDT_T4<WDT1> wdt;

uint32_t  lastResetCause = 0; 

void myCallback() {
  Serial.println("FEED THE DOG SOON, OR RESET!");
}

void setup() {

  Serial.begin(115200);
  while(!Serial);

  // Save copy of Reset Status register 
  lastResetCause = SRC_SRSR;
  
  // Clear all reset status register bits with
  SRC_SRSR = (uint32_t)0x7F;
  
  /*
  // Or sequentially clear the reset status of some interesting register bits 
  SRC_SRSR = SRC_SRSR_IPP_RESET_B;        // (1 << 0)  Power on bit
  SRC_SRSR = SRC_SRSR_LOCKUP_SYSRESETREQ; // (1 << 1)  after: SCB_AIRCR = 0x05FA0004;
                                          // (After CPU lockup or software setting of SYSRESETREQ bit)
  SRC_SRSR = SRC_SRSR_WDOG_RST_B;         // /1 << 4)  Watchdog 1 Reset bit
  */
  
  char buf[100] {0};
  sprintf(buf, "0x%08x", SRC_SRSR);
  Serial.println("Showing cleared Reset Staus Register:");
  Serial.println(buf);
  
  // Get interesting bits
  bool powerResetHappened = ((lastResetCause & SRC_SRSR_IPP_RESET_B) != 0);           //  Power on ?
  bool softwareResetHappened = ((lastResetCause & SRC_SRSR_LOCKUP_SYSRESETREQ) != 0); //  Software Reset through setting of SYSRESETREQ bit ?
  bool watchDogResetHappened = ((lastResetCause & SRC_SRSR_WDOG_RST_B) !=0);          //  Watchdog ?

  // Print Reset Causes since last clearing of Reset Staus Register
  Serial.println("LastResetCauseRegister:");
  
  sprintf(buf, "0x%08x", lastResetCause);
  Serial.println(buf);
  Serial.println("");
  Serial.println("Reset happened after:");

  if (powerResetHappened) {
    Serial.println("Power Reset.");
  }

  if (softwareResetHappened) {
    Serial.println("Software Reset.");
  }

  if (watchDogResetHappened) {
    Serial.println("Watchdog Reset.");
  }

  delay(2000);

  WDT_timings_t config;
  config.trigger = 5; /* in seconds, 0->128 */
  config.timeout = 10; /* in seconds, 0->128 */
  config.callback = myCallback;
  wdt.begin(config);
  
  pinMode(13, OUTPUT);

  // Reset Teensy 4.1, uncomment to activate software reset
  // SCB_AIRCR = 0x05FA0004;

   delay(1000);

}

void loop() {
  static uint32_t blinkled = millis();
  if ( millis() - blinkled > 50 ) {
    blinkled = millis();
    digitalWriteFast(13, !digitalReadFast(13));
  }

  static uint32_t callback_test = millis();
  if ( millis() - callback_test > 5500 ) {
    callback_test = millis();
    //wdt.feed(); /* uncomment to feed the watchdog */
  }
}

