#ifndef RS_ARDUINO_FRITZ_API_H
#define RS_ARDUINO_FRITZ_API_H

#include <Arduino.h>
#include <MD5.h>
#include "EthernetHttpClient_SSL.h"
#include "NativeEthernet.h"
#include "Ethernet_HTTPClient/Ethernet_HttpClient.h"
#include "SSLClient/SSLClient.h"
//#include "defines.h"
//#include "EthernetWebServer_SSL.h"
//#include "functional-vlpp.h"
//#include "QNEthernet.h"
//#include "QNEthernetClient.h"
//#include <HTTPClient.h>

#define FRITZ_ERR_HTTP_COMMUNICATION  -1001
#define FRITZ_ERR_NO_CHALLENGE        -1002
#define FRITZ_ERR_NO_SID              -1003
#define FRITZ_ERR_EMPTY_SID           -1004
#define FRITZ_ERR_INVALID_SID         -1005
#define FRITZ_ERR_VALUE_NOT_AVAILABLE -1006

#define FRITZ_ON_TEMPERATURE          100.0f
#define FRITZ_OFF_TEMPERATURE         0.0f

typedef enum {
      useHttp,
      useHttps
  } Protocol;

 

class FritzApi {
  public:
    // Constructor: FB user, FB password, FB address (ip or 'fritz.box')
    //FritzApi(const char* user, const char* password, Protocol protocol, EthernetHttpClient * pHttp);
    FritzApi(const char* user, const char* password, Protocol protocol, EthernetClient * client, EthernetSSLClient * sslClient, EthernetHttpClient * httpClient);
    //FritzApi(const char* user, const char* password, Protocol protocol);
    ~FritzApi();

    void init();

    
	
	// Switch actor on, return new switch state (true=on, false=off)
    boolean setSwitchOn(String ain);
	// Switch actor off, return new switch state (true=on, false=off)
    boolean setSwitchOff(String ain);
	// Toggle switch state, return new switch state (true=on, false=off)
    boolean setSwitchToggle(String ain);
	// Retrieve current switch state (true=on, false=off)
    boolean getSwitchState(String ain);
	// Check for presence of a switch with the given ain (true=present, false=offline)
    boolean getSwitchPresent(String ain);
	// Get current power consumption in Watt
	double getSwitchPower(String ain);
	// Get total energy since last reset in Wh
	int getSwitchEnergy(String ain);
	// Get temperature in °C
    double getTemperature(String ain);
	// Get name of an actor
	String getSwitchName(String ain);

	// Get nominal temperature of thermostat (8 = <= 8°C, 28 = >= 28°C, 100 = max, 0 = off)
	double getThermostatNominalTemperature(String ain);
	// Get comfort temperature of thermostat (8 = <= 8°C, 28 = >= 28°C, 100 = max, 0 = off)
	double getThermostatComfortTemperature(String ain);
	// Get reduced temperature of thermostat (8 = <= 8°C, 28 = >= 28°C, 100 = max, 0 = off)
	double getThermostatReducedTemperature(String ain);
	// Set nominal temperature of thermostat (8 = <= 8°C, 28 = >= 28°C, 100 = max, 0 = off)
	double setThermostatNominalTemperature(String ain, double temp);



  private:
    Protocol _protocol;
    const char* _user;
    const char* _pwd;
    const char* _ip;
    String _sid;

    byte mynewbytes[100];



    EthernetHttpClient * http;
	EthernetClient * client;
    EthernetSSLClient * sslClient;
  //static EthernetSSLClient sslClient(client, TAs, (size_t)TAs_NUM, 1, EthernetSSLClient::SSL_DUMP);   // Define Log Level
  //static EthernetSSLClient sslClient(client, TAs, (size_t)TAs_NUM);


    //HTTPClient http;

    


    String getChallengeResponse();
    String getSID(String response);
    String executeRequest(String request);
	double convertTemperature(String result);
    
};

#include "RsArduinoFritzApi_Impl.h"

#endif // RS_ARDUINO_FRITZ_API_H