// Garden Sensor Datalogger
// 2022-03-21
// 
// This program is used to log sensor data for the home garden. 
// The program is run on a microcontroller connected to the following sensors:
// Capacitive Soil Moisture Sensor v2.0 - soil moisture,
// TEMT6000 - light intensity,
// BH1750 - light intensity,
// DHT22 - temperature and humidity,
// BMP280 - barometric pressure, and
// MicroSD card - stores the data. 
//
// The Soil Moisture Sensor and TEMT6000 are analog signals through ADC.
// The DHT22 is a digital signal.
// The BH1750 and BMP280 are connected via I2C. 
// The MicroSD card reader is connected using SPI.
//
// The datalogger runs once per hour. 
// The data is stored to the MicroSD card as a CSV file.


// Libraries:
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <BH1750.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>


// Global variables:
const int pinSDCard = PA4;
const int pinTEMT6000 = PA1;
const int pinSoilMoisture = PA2;
const int pinDHT22 = PB9;
const int sampleDelay = 1000;     // Sets the delay between samples in milliseconds.


// Classes:
Adafruit_BMP280 bmp;
BH1750 lightMeter;
DHT dht(pinDHT22, DHT22);


void setup(void) {

  Serial.begin(115200);
  Wire.begin();
   
  // Initialize the MicroSD card.
  Serial.print("Initializing SD card...");
  if (!SD.begin(pinSDCard)) {
    Serial.println("Failed to find the SD card.");
  }
  Serial.println("Card initialized.");

  // Initialize the TEMT6000 sensor.
  pinMode(pinTEMT6000,  INPUT); 
  Serial.println("TEMT6000 initialized.");

  // Initialize the BMP280 sensor.
  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
    Serial.println("Failed to find the BMP280 sensor.");
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Operating Mode 
                  Adafruit_BMP280::SAMPLING_X2,     // Temp. oversampling 
                  Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling 
                  Adafruit_BMP280::FILTER_X16,      // Filtering 
                  Adafruit_BMP280::STANDBY_MS_1);   // Standby time
  Serial.println("BMP280 initialized.");

  // Initialize the BH1750 sensor.
  if (!lightMeter.begin()) {
    Serial.println("Failed to find the BH1750 sensor.");
  }
  Serial.println("BH1750 initialized.");

  // Initialize the DHT22 sensor.
  dht.begin();
  Serial.println("DHT22 initialized.");

  // Initialize the Capacitive Soil Moisture sensor.
  pinMode(pinSoilMoisture,  INPUT); 
  Serial.println("Capacitive Soil Moisture sensor initialized.");

  // Create the field headings within the data file on the MicroSD card.
  File dataFile = SD.open("DATALOG.TXT", FILE_WRITE);

  if (dataFile) {
    dataFile.println("Microseconds, BH1750-Light (lx), TEMT6000-Light (%), "
                      "BMP280-Temperature (C), BMP280-Air Pressure (Pa),"
                      "DHT22-Temperature (C), DHT22-Humidity (%), Soil-Moisture (%),");
    dataFile.close();
  }
  else {
    Serial.println("Error opening the log file.");
  }

  // Report successful completion of initialization stage.
  Serial.println("Initialization successful.");
  
}


void loop(void) {

  // Print the data to Serial.
  Serial.print(micros());
  Serial.print(",");
  Serial.print(lightMeter.readLightLevel(),2);
  Serial.print(",");
  Serial.print((analogRead(pinTEMT6000) / 1023.0 * 100.0),2);
  Serial.print(",");
  Serial.print(bmp.readTemperature(),2);
  Serial.print(",");
  Serial.print(bmp.readPressure(),2);
  Serial.print(",");
  Serial.print(dht.readTemperature(),2);
  Serial.print(",");
  Serial.print(dht.readHumidity(),2);
  Serial.print(",");
  Serial.print((analogRead(pinSoilMoisture) / 1023.0 * 100.0),2);
  Serial.println(",");                           
    
  // Open the file. If the file is available, write to it. If not, report an error.
  File dataFile = SD.open("DATALOG.TXT", FILE_WRITE);

  if (dataFile) {
    dataFile.print(micros());
    dataFile.print(",");
    dataFile.print(lightMeter.readLightLevel(),2);
    dataFile.print(",");
    dataFile.print((analogRead(pinTEMT6000) / 1023.0 * 100.0),2);
    dataFile.print(",");
    dataFile.print(bmp.readTemperature(),2);
    dataFile.print(",");
    dataFile.print(bmp.readPressure(),2);
    dataFile.print(",");
    dataFile.print(dht.readTemperature(),2);
    dataFile.print(",");
    dataFile.print(dht.readHumidity(),2);
    dataFile.print(",");
    dataFile.print((analogRead(pinSoilMoisture) / 1023.0 * 100.0),2);
    dataFile.println(",");                           
    dataFile.close();
  }
  else {
    //Serial.println("Error opening the log file.");
  }
  
  // Temporary delay. Replace with sleep function.
  delay(sampleDelay);
    
}
