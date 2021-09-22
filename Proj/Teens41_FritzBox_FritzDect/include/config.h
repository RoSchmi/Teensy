#include <Arduino.h>

#ifndef _CONFIG_H
#define _CONFIG_H

// This file is for 'not secret' user specific configurations
//
// Please set your timezone offset (time difference from your zone 
// to UTC time in units of minutes) and set the time difference
// used for DaylightSaving Time in minutes
// Define begin and end of Daylightsaving 
//
// Please select the transport protocol, http or https (https is recommended)
// For https you must include the Root Certificate of your Azure Account
// like here the baltimore_root_ca
// Select the Sendinterval in minutes
// Select the Invalidate Interval in minutes (Sensor values not actualized
// within this interval are considered to be invalid)
// Define other settings according to your needs

//
// The credentials of your WiFi router (not needed for Teensy 4.1) and the name and key of your
// Azure Storage Account have to be set in the file config_secret.h





#define TRANSPORT_PROTOCOL 0            // 0 = http, 1 = https

#define SENDINTERVAL_MINUTES   0.2          // Sendinterval in minutes (10 is recommended), in this interval                                        
                                                   // data are sent to the Cloud (is limited to be not below 1 second)

#define SERIAL_PRINT 1                      // 1 = yes, 0 = no. Select if Serial.print messages are printed 
                                           

// Names for Tables in Azure Account, please obey rules for Azure Tablenames (e.g. no underscore allowed)
// regular expression "^[A-Za-z][A-Za-z0-9]{2,62}$".
// max length in this App is 45

#define ANALOG_TABLENAME "AnalogValuesX"          // Name of the Azure Table to store 4 analog Values max length = 45

#define ANALOG_TABLE_PART_PREFIX "Y2_"            // Prefix for PartitionKey of Analog Tables (default, no need to change)


                                               // Names of tables to be created in your Azure Storage Account
                                               // Per default the names are augmented with the actual year in this App
#define ON_OFF_TABLENAME_01 "OnOffx01x"          // Name of the 1. On/Off Table  max length = 45
#define ON_OFF_TABLENAME_02 "OnOffx02x"          // Name of the 2. On/Off Table  max length = 45
#define ON_OFF_TABLENAME_03 "OnOffx03x"          // Name of the 3. On/Off Table  max length = 45
#define ON_OFF_TABLENAME_04 "OnOffx04x"          // Name of the 4. On/Off Table  max length = 45

#define ON_OFF_TABLE_PART_PREFIX "Y3_"           // Prefix for PartitionKey of On/Off Tables (default, only change if needed)

#define INVALIDATEINTERVAL_MINUTES 10      // Invalidateinterval in minutes 
                                           // (limited to values between 1 - 60)
                                           // (Sensor readings are considered to be invalid if not successsfully
                                           // read within this timespan)

#define NTP_UPDATE_INTERVAL_MINUTES  10    //  With this interval sytem time is updated via NTP
                                           //  with internet time (is limited to be not below 1 min)

#define UPDATE_TIME_FROM_AZURE_RESPONSE 0  // 1 = yes, 0 = no. SystemTime is updated from the Post response from Azure.
                                           // With this option set, you can set  NTP_UPDATE_INTERVAL_MINUTES to a very
                                           // long time, so that it 'never' happens                                      

#define ANALOG_SENSOR_READ_INTERVAL_SECONDS 2   // Analog sensors are read with this interval  (limited 1 to 14400)                                    

#define WORK_WITH_WATCHDOG 1             // 1 = yes, 0 = no, Watchdog is used (1) or not used (0)
                                           // should be 1 for normal operation and 0 for testing
                                           

                                            
// Set timezoneoffset and daylightsavingtime settings according to your zone
// https://en.wikipedia.org/wiki/Daylight_saving_time_by_country
// https://en.wikipedia.org/wiki/List_of_time_zone_abbreviations

#define TIMEZONEOFFSET 60           // TimeZone time difference to UTC in minutes
#define DSTOFFSET 60                // Additional DaylightSaving Time offset in minutes

#define  DST_ON_NAME                "CEST"
#define  DST_START_MONTH            "Mar"    // Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov or Dec
#define  DST_START_WEEKDAY          "Sun"    // Sun, Mon, Tue, Wed, Thu, Fri, Sat
#define  DST_START_WEEK_OF_MONTH    "Fourth"   // Last, First, Second, Third, Fourth, 
#define  DST_START_HOUR              2        // 0 - 23

#define  DST_OFF_NAME               "CET"
#define  DST_STOP_MONTH             "Oct"    // Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov or Dec
#define  DST_STOP_WEEKDAY           "Sun"    // Sun, Tue, Wed, Thu, Fri, Sat
#define  DST_STOP_WEEK_OF_MONTH     "Last"   // Last, First, Second, Third, Fourth
#define  DST_STOP_HOUR               3       // 0 - 23
       


                                 

#define USE_STATIC_IP 0                // 1 = use static IpAddress, 0 = use DHCP
                                        // for static IP: Ip-addresses have to be set in the code



#endif // _CONFIG_H