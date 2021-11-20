#ifndef COMMS_H
#define COMMS_H

class Communications{
  public:
    //Maybe pseudo-thread this?
    bool IsInSearchArea(int timeout=1000){
        SerialNina.println("InSearch");
        unsigned long start_time = millis();
        while(millis() - start_time < timeout){
          if(SerialNina.available()){
            char c = SerialNina.read();
            switch(c){
              case 'y': return true;
              default: return false;
            }
          }
        }
        return false;
    }
    
};


#endif