#include <Arduino.h>

#ifndef _CONFIG_H
#define _CONFIG_H

// This file is for 'not secret' user specific configurations

// Secret user specific configurations have to be set 
// in the file config_secret.h


#define TRANSPORT_PROTOCOL 0           // 0 = http, 1 = https
                           
#define USE_STATIC_IP 0                // 1 = use static IpAddress, 0 = use DHCP
                                       // for static IP: Ip-addresses have to be set in the code


#endif // _CONFIG_H