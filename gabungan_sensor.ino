#include <BH1750.h> // Light Sensor Library
#include <Wire.h> // I2C Library
#include <DHT.h> // Temp Sensor Library
#include <HardwareSerial.h> //Connection to pzem
#include <PZEM004Tv30.h> //Pzem Library

BH1750 lightMeter; //Light sensor declaration

const int DHTPIN = 23;     // Temp sensor pin
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

float hum;  //Stores humidity value
float temp; //Stores temperature value

HardwareSerial S(2);
PZEM004Tv30 pzem(&S); //Pzem declaration

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(); //I2C Light sensor, pin 22 and 21
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire); //I2C Address and light sensor mode
  dht.begin();
  Serial.println("Test");
}

void readLight(){
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.println(lux);
  //delay(500);
}

void readTemp(){
  hum = dht.readHumidity();
  temp= dht.readTemperature();
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Serial.println(" Celsius\n");
  //delay(2000);
}

void readPzem(){
  float volt = pzem.voltage();
  Serial.print("Voltage: ");
  Serial.print(volt);
  Serial.println("V");

  float cur = pzem.current();
  Serial.print("Current: ");
  Serial.print(cur);
  Serial.println("A");

  float powe = pzem.power();
  Serial.print("Power: ");
  Serial.print(powe);
  Serial.println("W");

  float ener = pzem.energy();
  Serial.print("Energy: ");
  Serial.print(ener,3);
  Serial.println("kWh");

  Serial.println("\n--------------");

  //delay(1000);
}

void loop(){
  readLight();
  readTemp();
  readPzem();
  delay(2000);
}
