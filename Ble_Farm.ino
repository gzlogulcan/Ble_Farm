#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 
#define RE 8
#define DE 7
 
const byte nitro_inquiry_frame[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos_inquiry_frame[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota_inquiry_frame[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
const byte ph[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
byte values[11];

const byte ec[] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};
const byte salinity[] = {0x01, 0x03, 0x00, 0x14, 0x00, 0x01, 0xC4, 0x0E};
byte valuesec[8];

SoftwareSerial modbus(2,3);

void setup() {
  Serial.begin(9600);
  modbus.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
}
 
void loop() {
 
  byte nitrogen_val,phosphorus_val,potassium_val;
  nitrogen_val = nitrogen();
  delay(250);
  phosphorus_val = phosphorous();
  delay(250);
  potassium_val = potassium();
  delay(250);

  /**************Soil EC Reading*******************/  
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
 
  if (modbus.write(ec, sizeof(ec)) == 8)
  {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++)
    {
      values[i] = modbus.read();
      //Serial.print(valuesec[i], HEX);
    }
    Serial.println();
  }
  int soil_ec = int(valuesec[3]<<8|valuesec[4]);
  delay(1000);
/**************Soil Salinity Reading*******************/ 
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (modbus.write(salinity, sizeof(salinity)) == 8)
  {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++)
    {
      valuesec[i] = modbus.read();
      //Serial.print(valuesec[i], HEX);
    }
    //Serial.println();
  }
  int soil_salinity = int(valuesec[4]);
  delay(1000);

  
  Serial.println("Agriculture Technology ");
  Serial.println();
  Serial.print("Nitrogen Value: ");
  Serial.print(nitrogen_val);
  Serial.println(" mg/kg");
  Serial.print("Phosphorous Value: ");
  Serial.print(phosphorus_val);
  Serial.println(" mg/kg");
  Serial.print("Potassium Value: ");
  Serial.print(potassium_val);
  Serial.println(" mg/kg");
  Serial.print("Soil EC: ");
  Serial.println(soil_ec);
  Serial.print("Soil Salinity: ");
  Serial.println(soil_salinity);
  //Serial.println("-------------------------");

  
  byte val;
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  if (modbus.write(ph, sizeof(ph)) == 8)
  {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 11; i++)
    {
      values[i] = modbus.read();
      //Serial.print(values[i], HEX);
    }
  }
  float soil_ph = float(values[4]) / 10;
  Serial.print("Ph Value: ");
  Serial.println(soil_ph, 1);
 delay(2000);
}
 
byte nitrogen(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(modbus.write(nitro_inquiry_frame,sizeof(nitro_inquiry_frame))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(modbus.read(),HEX);
    values[i] = modbus.read();
   // Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4]; 
}
 
byte phosphorous(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(modbus.write(phos_inquiry_frame,sizeof(phos_inquiry_frame))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(modbus.read(),HEX);
    values[i] = modbus.read();
   // Serial.print(values[i],HEX);
    }
  }
  return values[4];
}
 
byte potassium(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  
  if(modbus.write(pota_inquiry_frame,sizeof(pota_inquiry_frame))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(modbus.read(),HEX);
    values[i] = modbus.read();
    //Serial.print(values[i],HEX);
    }
    delay(10);
    Serial.println("-------------------------");
  }
  return values[4];
}
