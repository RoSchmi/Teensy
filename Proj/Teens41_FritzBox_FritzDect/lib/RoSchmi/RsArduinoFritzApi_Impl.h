//#include <MD5Builder.h>
#include <Arduino.h>
#include "RsArduinoFritzApi.h"
#include "EthernetHttpClient_SSL.h"
//#include "EthernetWebServer_SSL.h"
//#include "defines.h"

//#include "NativeEthernetClient.h"
// TODO: Add Compiler Directive for logging stuff ... #define FRITZ_API_DEBUG

//
//FritzApi::FritzApi(const char* user, const char* password, Protocol protocol, EthernetHttpClient * pHttp) {
FritzApi::FritzApi(const char* user, const char* password, Protocol protocol, EthernetClient * pClient, EthernetSSLClient * pSslClient, EthernetHttpClient * pHttp) {
  _user = user;
  _pwd = password;
  //_ip = ip;
  _protocol = protocol;
  http = pHttp;
  client = pClient;
  sslClient = pSslClient;


  //sslClient(client, TAs, (size_t)TAs_NUM, 1, EthernetSSLClient::SSL_DUMP);
}

FritzApi::~FritzApi(){
  _user="";
  _pwd="";
  _ip="";
  _protocol = Protocol::useHttps;
  http = nullptr;
  client = nullptr;
  sslClient = nullptr;


}


void FritzApi::init() {

     String response = getChallengeResponse();
     //String response = "Hallo";
     _sid = getSID(response);
     Serial.println("SID: " + _sid);
     /*
     while (true)
       {
         delay(100);
       }
      */
      /*
      if (_sid == "") 
      {
          Serial.println("FRITZ_ERR_EMPTY_SID");
	        //throw FRITZ_ERR_EMPTY_SID;
      } 
      else if (_sid == "0000000000000000") 
      {
        Serial.println("FRITZ_ERR_INVALID_SID");
  	    //throw FRITZ_ERR_INVALID_SID;   	
      }
      */
}



String FritzApi::getChallengeResponse() {
   // Get Challenge
   // RoSchmi

   //EthernetHttpClient http()   sslClient, (char *)FRITZ_IP_ADDRESS, (TRANSPORT_PROTOCOL == 0) ? 80 : 443))

   if (http->connect((char *)"fritz.box", 80))
   {
     Serial.println(F("Connected to FritzBox"));
     
     //http->beginRequest();
     //Serial.println("Returned from begin request");
     
     //int retCode = http->get("http://" + String("192.168.1.1") + "/login_sid.lua");
     int retCode = http->get("/login_sid.lua");
     Serial.println(F("Returned from get"));
     Serial.println(retCode);
     if (retCode != 0)
     {
       Serial.println(F("Error on http communication"));
     }
     else
     {
       Serial.println(F("http communication successful"));
     }
     int statusCode = http->responseStatusCode();
     Serial.println("StatusCode:");
     Serial.println(statusCode);
     String result = "";
     if (statusCode != 200)
     {
       Serial.println(F("FRITZ_ERR_NO_CHALLENGE"));
       result = http->responseBody();
       while (true)
       {
         Serial.println(F("FRITZ_ERR_NO_CHALLENGE"));
         delay(1000);
       }
     }
     else
     {
       Serial.println(F("FRITZ_GOT_CHALLENGE"));
       result = http->responseBody();
       String result2 = result.substring(0, result.length() - 1);
       Serial.println(result2);
       String challenge = result.substring(result.indexOf("<Challenge>") + 11, result.indexOf("</Challenge>"));
       String challengeReponse = challenge + "-" + String(_pwd);
       String responseHash = "";
      
       char meinMD5String[challengeReponse.length() + 1] = {} ;
        challengeReponse.toCharArray(meinMD5String, challengeReponse.length() + 1);
        //Nach 16Bit Konvertieren
        int i = 0;
        size_t x = 0;
        while ( x < strlen(meinMD5String))
        {
          mynewbytes[i] = meinMD5String[x];
          i++;
          mynewbytes[i] = 0x00;
          i++;
          x++;
        }
        //http->flush();
        http->stop();
  //MD5 Verarbeitung mit chid+passwort MD5
  //https://github.com/tzikis/ArduinoMD5/blob/master/examples/MD5_Hash/MD5_Hash.ino
  //https://github.com/tzikis/ArduinoMD5/
        
        unsigned char* hash = MD5::make_hash((char*)mynewbytes, (size_t)(strlen((char *)meinMD5String) * 2));
        char *md5str = MD5::make_digest(hash, 16);
        free(hash);        
        Serial.println(F("This is the MD5 String:"));
        Serial.println(md5str);
        
        
        delay(200);
        //return challengeReponse;
        return challenge + "-" + md5str;
     
   }

}
else
{
  Serial.println("Not connected");
  return "";
}
}

   //http.begin("http://" + String(_ip) + "/login_sid.lua");
   /*
   int retCode = http.GET();
   if (retCode < 0) {
	 throw FRITZ_ERR_HTTP_COMMUNICATION;
   } else if (retCode != 200) {
	 throw FRITZ_ERR_NO_CHALLENGE;
   }
   String result = http.getString();
   String challenge = result.substring(result.indexOf("<Challenge>") + 11, result.indexOf("</Challenge>"));
   String challengeReponse = challenge + "-" + String(_pwd);
   String responseHash = "";
   */
   
   /*
   for (unsigned int i = 0; i  < challengeReponse.length(); i++) {
     // TODO: Sorry, no real UTF-16LE for now... just add 00
     responseHash = responseHash + String(challengeReponse.charAt(i), HEX) + "00";
   }

   MD5Builder md5;
   md5.begin();
   md5.addHexString(responseHash);
   md5.calculate();
   
   return challenge + "-" + md5.toString();
   
}
*/


String FritzApi::getSID(String response) {
  //http.begin("http://" + String(_ip) + "/login_sid.lua?user=" + String(_user) + "&response=" + response);
  Serial.println(F("Going to get SID, connState:"));

  uint8_t connState = http->connected();
  Serial.println(connState);
  
  /*
  if (connState == 0)
  {
    Serial.println(F("Have to connect"));
    if (!(http->  connect((char *)"fritz.box", 80)))
    {
      Serial.println("Error on connection to fritzbox");
      return "";
    }
  }
  */

  Serial.println("Going to get SID, connected");
  
  Serial.println(_user);
  int retCode = -1;

  char aUrlPa[140] {0};
  //sprintf((char *)aUrlPa, "%s%s", "http://fritz.box/login_sid.lua?username=&response=", (char *)response.c_str());
  sprintf((char *)aUrlPa, "%s%s%s%s", "/login_sid.lua?user=", _user, "&response=", (char *)response.c_str());


  String augUrlPath = aUrlPa;
  //String augUrlPath = "http://192.168.1.1/login_sid.lua?username=&response=" + response;

  //String augUrlPath = "/login_sid.lua";
  Serial.println(augUrlPath);
    //retCode = http->get("/login_sid.lua?user=" + String(_user) + "&response=" + response);
    
    //http->beginRequest();
    http->connect((char *)"fritz.box", 80);
    retCode = http->get(augUrlPath);
  
    Serial.println(F("Returned from get"));
     Serial.println(retCode);
     if (retCode != 0)
     {
       Serial.println(F("Error on http communication"));
       return "";
     }
     Serial.println(F("Getting StatusCode:"));
     uint32_t start = millis();
     while ((uint32_t)(millis - start) < 500)
     {
        delay(1);
     }

     int statusCode = http->responseStatusCode();
    
     Serial.println(F("StatusCode:"));
     Serial.println(statusCode);
     while (true)
     {
       delay (500);
     }
  
     //int retCode = http.GET();
     if (statusCode != 200) {
      Serial.println(F("FRITZ_ERR_NO_SID"));
      return "";
      }
      String result = http->responseBody();
      // TODO: check indexOf > 0
      String sid = result.substring(result.indexOf("<SID>") + 5,  result.indexOf("</SID>"));
  return sid;
}


/*
boolean FritzApi::setSwitchOn(String ain) {
  String result = executeRequest("setswitchon&sid=" + _sid + "&ain=" + String(ain));
  return (atoi(result.c_str()) == 1);
}
*/

/*
boolean FritzApi::setSwitchOff(String ain) {
  String result = executeRequest("setswitchoff&sid=" + _sid + "&ain=" + String(ain));
  return (atoi(result.c_str()) == 1);
}
*/

/*
boolean FritzApi::setSwitchToggle(String ain) {
  String result = executeRequest("setswitchtoggle&sid=" + _sid + "&ain=" + String(ain));
  return (atoi(result.c_str()) == 1);
}
*/

/*
boolean FritzApi::getSwitchState(String ain) {
  String result = executeRequest("getswitchstate&sid=" + _sid + "&ain=" + String(ain));
  return (atoi(result.c_str()) == 1);
}
*/

/*
boolean FritzApi::getSwitchPresent(String ain) {
  String result = executeRequest("getswitchpresent&sid=" + _sid + "&ain=" + String(ain));
  return (atoi(result.c_str()) == 1);
}
*/

/*
double FritzApi::getSwitchPower(String ain) {
  String result = executeRequest("getswitchpower&sid=" + _sid + "&ain=" + String(ain));
  if (result == "inval") {
  	throw FRITZ_ERR_VALUE_NOT_AVAILABLE;
  }
  return atof(result.c_str()) / 1000;
}
*/

/*
int FritzApi::getSwitchEnergy(String ain) {
  String result = executeRequest("getswitchenergy&sid=" + _sid + "&ain=" + String(ain));
  if (result == "inval") {
  	throw FRITZ_ERR_VALUE_NOT_AVAILABLE;
  }
  return atoi(result.c_str());
}
*/

/*
double FritzApi::getTemperature(String ain) {
  String result = executeRequest("gettemperature&sid=" + _sid + "&ain=" + String(ain));
  if (result == "inval") {
  	throw FRITZ_ERR_VALUE_NOT_AVAILABLE;
  }
  return atof(result.c_str()) / 10;
}
*/

/*
String FritzApi::getSwitchName(String ain) {
  return executeRequest("getswitchname&sid=" + _sid + "&ain=" + String(ain));
}
*/

/*
double FritzApi::getThermostatNominalTemperature(String ain) {
  String result = executeRequest("gethkrtsoll&sid=" + _sid + "&ain=" + String(ain));
  if (result == "inval") {
  	throw FRITZ_ERR_VALUE_NOT_AVAILABLE;
  }
  return convertTemperature(result);
}
*/

/*
double FritzApi::getThermostatComfortTemperature(String ain) {
  String result = executeRequest("gethkrkomfort&sid=" + _sid + "&ain=" + String(ain));
  if (result == "inval") {
  	throw FRITZ_ERR_VALUE_NOT_AVAILABLE;
  }
  return convertTemperature(result);
}
*/

/*
double FritzApi::getThermostatReducedTemperature(String ain) {
  String result = executeRequest("gethkrabsenk&sid=" + _sid + "&ain=" + String(ain));
  if (result == "inval") {
  	throw FRITZ_ERR_VALUE_NOT_AVAILABLE;
  }
  return convertTemperature(result);
}
*/

/*
double FritzApi::setThermostatNominalTemperature(String ain, double temp) {
  int param;
  if (temp == FRITZ_OFF_TEMPERATURE) {
    param=253;
  } else if (temp == FRITZ_ON_TEMPERATURE) {
    param=254;
  } else if (temp < 8.0f) {
    param=16;
  } else if (temp > 28.0f) {
    param=56;
  } else {
    param = 2*temp;
  }
  String result = executeRequest("sethkrtsoll&sid=" + _sid + "&ain=" + String(ain) + "&param=" + param);
  return temp;
}
*/

/*
String FritzApi::executeRequest(String request) {
  String result;
  int httpStatus;
  for (int retries = 0; retries < 3; retries++) {
    http.begin("http://" + String(_ip) + "/webservices/homeautoswitch.lua?switchcmd=" + request);
    httpStatus = http.GET();
    if (httpStatus == 200) {
      return http.getString();
    } else {
      // Try to get new Session
      init();
    }
  }
  if (httpStatus < 0) {
    throw FRITZ_ERR_HTTP_COMMUNICATION;
  } else {
    throw httpStatus;
  }
}
*/

/*
double FritzApi::convertTemperature(String result) {
  double temp = atof(result.c_str());
  if (temp == 254) {
    return FRITZ_ON_TEMPERATURE;
  } else if (temp == 253 ){
    return FRITZ_OFF_TEMPERATURE;
  }
  return temp/2;
}
*/