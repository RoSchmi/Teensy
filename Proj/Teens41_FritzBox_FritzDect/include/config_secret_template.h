#ifndef _CONFIG_SECRET_H
#define _CONFIG_SECRET_H

// Rename this file to config_secret.h to instantiate the user
// specific settings

// This file is for 'secret' user specific configurations

// Not secret user specific configurations have to be set 
// in the file config.h

#define FRITZ_IP_ADDRESS "fritz.box"    // IP Address of FritzBox
                                        // Change for your needs
#define FRITZ_USER ""                   // FritzBox User (may be empty)
#define FRITZ_PASSWORD "MySecretName"   // FritzBox Password

#define FRITZ_DEVICE_AIN_01 ""    // AIN = Actor ID Numberof Fritz!Dect (12 digit)
#define FRITZ_DEVICE_AIN_02 ""             
#define FRITZ_DEVICE_AIN_03 ""
#define FRITZ_DEVICE_AIN_04 ""

#endif // _CONFIG_SECRET_H