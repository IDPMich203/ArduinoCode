#ifndef DUMMY_H
#define DUMMY_H

//Returns 0 for no dummy, 1,2,3 order on moodle
int detectDummy(){
  int qsdPin = A1;
  int qsdVal = 0;
  float qsdAvg = 0;
  long int itt = 0;
  int tsopPin = A0;
  int tsopVal = 0;
  float tsopAvg = 0;
for(itt =0; itt < 10000; itt++){
//  if(itt % 1000 == 0){
//  yield();
//  }
//  itt = itt + 1;
  qsdVal = analogRead(qsdPin);
  qsdAvg = qsdAvg + qsdVal;
  tsopVal = analogRead(tsopPin);
  tsopAvg = tsopAvg + tsopVal;
}

  qsdAvg = qsdAvg/10000;
  tsopAvg = tsopAvg/10000;

  Serial.print("QSDAvg: ");
  Serial.println(qsdAvg);

  Serial.print("TSOPAvg: ");
  Serial.println(tsopAvg);

  if(qsdAvg>980){
    //No dummy
    return 0;
  }
  if(tsopAvg < 920){
    //38kHz only
    return 1;
  }            
  else if(tsopAvg > 1000){
    //600us only
    return 2;
  }
  else{
    //Half and half
    return 3;
  }
  
}

#endif
