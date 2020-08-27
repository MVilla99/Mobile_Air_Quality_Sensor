/*
 * Project mobile_air_quality_testing_platform
 * Description:
 * Author:
 * Date:
 */
#include <Adafruit_SSD1306.h>
#include <Grove_Air_quality_Sensor.h>
#include <SdFat.h>
#include <SPI.h>
  /*  for sd card   */
unsigned long logTime;
  #define SD_CS_PIN SS
SdFat sd;
File file;
  #define FILE_BASE_NAME "aqdata"
char fileName[13] = FILE_BASE_NAME "00.csv";
const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) -1;
  #define error(msg) sd.errorHalt(msg)
  /* for air quality  */
AirQualitySensor Aq(A2); // put sensor pin in here
int quality;
int AQvalue;  

int aqRead;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  delay(100);
  Aq.init();
  if(!sd.begin(SD_CS_PIN)){
    Serial.println(" sd init failed");
    return;
  }
  Serial.println("sd init done");
  pinMode(A1, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, INPUT);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
int switchValue = digitalRead(A4);
if(switchValue == 1){
  digitalWrite(A3, HIGH);
  digitalWrite(A1,LOW);
  airQuality();
  file = sd.open("aqLog.txt", FILE_WRITE);
    if(file){
      Serial.println("file opened!");
      file.printf("air quality value: %i \n", aqRead);
      Serial.println("printing...");
      delay(10000);
      file.close();
    }
}
else if(switchValue == 0){
  digitalWrite(A1, HIGH);
  digitalWrite(A3, LOW);
  //Serial.println("off");
} 
}

void airQuality(){
  quality = Aq.slope();
  AQvalue = Aq.getValue();

  if(quality == AirQualitySensor::FORCE_SIGNAL){
    aqRead = 4;
  }
  else if(quality == AirQualitySensor::HIGH_POLLUTION){
    aqRead = 3;
  }
  else if(quality == AirQualitySensor::LOW_POLLUTION){
    // write to sd low pollution
    aqRead = 2;
  }
  else if(quality == AirQualitySensor::FRESH_AIR){
    // write fresh air to sd 
    aqRead = 1;
  }

}
