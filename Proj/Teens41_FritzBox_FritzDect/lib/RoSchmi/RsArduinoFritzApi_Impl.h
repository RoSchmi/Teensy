#include <Arduino.h>
#include "RsArduinoFritzApi.h"
#include "EthernetHttpClient_SSL.h"

FritzApi::FritzApi(const char* user, const char* password, const char * ip, Protocol protocol, EthernetClient * pClient, EthernetSSLClient * pSslClient, EthernetHttpClient * pHttp) {
  _user = user;
  _pwd = password;
  _ip = ip;
  _protocol = protocol; 
  _port = protocol == Protocol::useHttp ? 80 : 443;
  http = pHttp;
  client = pClient;
  sslClient = pSslClient;
}

FritzApi::~FritzApi(){
  _user="";
  _pwd="";
  _ip="";
  _protocol = Protocol::useHttps;
  _port = 443;
  http = nullptr;
  client = nullptr;
  sslClient = nullptr;
}

bool FritzApi::init() 
{
  String response = getChallengeResponse();
  if (response == "")
  {
    return false;
  }
  _sid = getSID(response);
  Serial.println("SID: " + _sid);
  if (_sid == "") 
  {
    Serial.println("FRITZ_ERR_EMPTY_SID");
    return false;
	  //throw FRITZ_ERR_EMPTY_SID;
  } 
  else if (_sid == "0000000000000000") 
  {
    Serial.println("FRITZ_ERR_INVALID_SID");
    return false;
  	//throw FRITZ_ERR_INVALID_SID;   	
  }
  return true;  
}

String FritzApi::getChallengeResponse() 
{
  http->connectionKeepAlive();
  
  if (!(http->connect((char *)_ip, _port)))
  {
    Serial.println(F("Not connected"));
    return "";
  }  
    int retCode = http->get("/login_sid.lua");
    if (retCode != 0)
    {
       Serial.println(F("Error on http communication"));
       return "";
    }
    
    int statusCode = http->responseStatusCode();
    
    String result = "";
    if (statusCode != 200)
    {
      Serial.println(F("FRITZ_ERR_NO_CHALLENGE"));
      result = http->responseBody();
      Serial.println(F("FRITZ_ERR_NO_CHALLENGE"));
      Serial.println(result);
      return "";
    }
    else
    {
      result = http->responseBody();    
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
      http->stop();

      //MD5 Verarbeitung mit chid+passwort MD5
      //https://github.com/tzikis/ArduinoMD5/blob/master/examples/MD5_Hash/MD5_Hash.ino
      //https://github.com/tzikis/ArduinoMD5/
        
      unsigned char* hash = MD5::make_hash((char*)mynewbytes, (size_t)(strlen((char *)meinMD5String) * 2));
      char *md5str = MD5::make_digest(hash, 16);
      free(hash);        
      //Serial.println(F("This is the MD5 String:"));
      //Serial.println(md5str);       
      //return challengeReponse;
      return challenge + "-" + md5str; 
   } 
}
  
String FritzApi::getSID(String response) 
{
   uint8_t connState = http->connected();
   Serial.print("ConnState: ");
   Serial.println(connState);
  
  /*
  if (http->connected() == 0)
  {  
      Serial.println("Error on connection to fritzbox");
      return "";
  }
  */
  char augUrlPath[140] {0};
  sprintf((char *)augUrlPath, "%s%s%s%s", "/login_sid.lua?user=", _user, "&response=", (char *)response.c_str());
    
  http->connect((char *)_ip, _port);

  int retCode = http->get(augUrlPath); 
  if (retCode != 0)
  {
    Serial.println(F("Error on http communication"));
    return "";
  }
  int statusCode = http->responseStatusCode();
  
  if (statusCode != 200) 
  {
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