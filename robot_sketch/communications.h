#ifdef COMMS_H
#define COMMS_H
#include "DelayAsync.h"
​
​
class Communications {
  public:
    WiFiClient client;
    bool Init(const char* SSID, const char* password, int timeout=10000) {
      unsigned long startTime = millis();
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delayAsync(500);
        Serial.println("...");
        if(millis() - startTime >= timeout){
          return false;
        }
      }
​
      Serial.print("WiFi connected with IP: ");
      Serial.println(WiFi.localIP());
      
      // Cheeky tactic where I reckon the server IP is always the gateway IP
      gateway = WiFi.gatewayIP();
​
      if (!client.connect(gateway, port)) {
        Serial.println("Connection to host failed");
        delayAsync(1000);
        return false;
      }
​
      
      Serial.println("Connected to server successful!");
      client.println("Connected");
      return true;
    }
​
​
    
    
    //Check if in search area
    bool IsInSearchArea(int timeout = 1000) {
      //Println so python detects end of command
      client.println("InSearch");
      //Flush to ensure everything is sent
      client.flush();
      //Wait till timeout
      
      unsigned long start_time = millis();
      while (millis() - start_time < timeout) {
​
        //Call tick to update any async functions such as blink
        MainTimer.tick();
        if (client.available()) {
          char c = client.read();
          switch (c) {
            case 'y': return true;
            default: return false;
          }
        }
      }
      return false;
    }
​
​
    //You can read to a string like so:
    /*
        while(client.available()){
        //Add to string and do checking stuff
          char c = client.read();
          msg += c;
          if(c == '\n'){
          //End of command. Do checking/parsing stuff
          }
        
        */
​
};

#endif
