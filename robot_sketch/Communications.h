#ifndef COMMS_H
#define COMMS_H
#include <WiFiNINA.h>
#include "DelayAsync.h"

#define PORT 8090

#define TIMEOUT 10000

#define RECV_BUFFER 16

class Communications {
  public:
    WiFiClient client;
    bool begin(const char* SSID, const char* password, int timeout = 10000) {
      unsigned long startTime = millis();
      WiFi.begin(SSID, password);
      while (WiFi.status() != WL_CONNECTED) {
        delayAsync(500);
        Serial.println("...");
        if (millis() - startTime >= timeout) {
          return false;
        }
      }

      Serial.print("WiFi connected with IP: ");
      Serial.println(WiFi.localIP());

      // Cheeky tactic where I reckon the server IP is always the gateway IP
      IPAddress gateway = WiFi.gatewayIP();

      if (!client.connect(gateway, PORT)) {
        Serial.println("Connection to host failed");
        delayAsync(1000);
        return false;
      }


      Serial.println("Connected to server successful!");
      client.println("Connected");
      return true;
    }




    //Check if in search area
    bool IsInSearchArea(bool &is, int timeout = 1000) {
      while(client.available()) client.read();
      //Println so python detects end of command
      client.println("InSearch");
      //Flush to ensure everything is sent
      client.flush();
      //Wait till timeout

      unsigned long start_time = millis();
      while (millis() - start_time < timeout) {

        //Call tick to update any async functions such as blink
        MainTimer.tick();
        if (client.available()) {
          char c = client.read();
          switch (c) {
            case 'y':
              is = true;
              return true;
            case 'n':
              is = false;
              return true;
            default: return false;
          }
        }
      }
      return false;
    }


    bool getBoxCoords(char box, int &x, int &y) {
      char boxfull[5];
      strcpy(boxfull, "box");
      boxfull[3] = box;
      boxfull[4] = 0;
      Serial.println(boxfull);
      return getCoords(boxfull, x, y);

    }

    bool getRobotCoords(int &x, int &y) {
      return getCoords("robotloc", x,y);
    }

    bool getRobotRotation(float &rot) {
      while(client.available()) client.read();
      String command;
      client.println("robotrot");
      client.flush();
      bool rcvd = recieve(command);
      if (!rcvd) {
        return false;
      }
      if (command.charAt(0) == 'F') {
        return false;
      }
      rot = command.toFloat();
      return true;
    }

    bool getDummyCoords(int &x, int &y) {
      return getCoords("dummy", x, y);

    }

    bool recieve(String &recieveStr, int timeout = TIMEOUT) {
      unsigned long start_time = millis();
      recieveStr = "";
      while (true) {
        if (millis() - start_time > timeout) {
          return false;
        }
        if (client.available()) {


          //Add to string and do checking stuff
          char c = client.read();
          recieveStr += c;
          if (c == '\n') {
            Serial.print("Got String: ");
            Serial.println(recieveStr);
            //End of command
            return true;
          }
        }

      }
    }

    bool getCoords(char* request,int &x,int &y) {
      while(client.available()) client.read();
      String command;
      client.println(request);
      client.flush();
      bool rcvd = recieve(command);
      if (!rcvd) {
        return false;
      }
      char data[RECV_BUFFER];
      command.toCharArray(data, RECV_BUFFER);
      if (command.charAt(0) == 'F') {
        return false;
      }
      char* ptr = strtok(data, ",");  // delimiter
      int index = 0;
      while (ptr != NULL)
      {

        if (index == 0) {
          x = atoi(ptr);
        } else if (index == 1) {
          y = atoi(ptr);
          return true;
        }
        index++;
        ptr = strtok(NULL, ",");
      }
    }

    void debug(char* message){
      client.print("m");
      client.println(message);
    }
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

};


#endif
