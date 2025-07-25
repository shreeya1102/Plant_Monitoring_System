
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Pin Definitions
#define DHT_PIN 2          // Pin connected to DHT11
#define MOISTURE_PIN A0    // Pin connected to Soil Moisture Sensor
#define LDR_PIN A1         // Pin connected to LDR
#define BUZZER_PIN 3       // Pin connected to Buzzer

// Threshold values
#define MOISTURE_THRESHOLD 600   // Below this value, the soil is considered dry

// Initialize DHT11 sensor
DHT dht(DHT_PIN, DHT11);

// Initialize LCD with I2C address (0x27 is common, use I2C scanner if unsure)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Start serial communication
  Serial.begin(9600);
  
  // Initialize DHT11 sensor
  dht.begin();
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setBacklight(1);
  
  // Initialize Buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);

  // Initial message on LCD
  lcd.print("Plant Monitor");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read temperature and humidity from DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Read soil moisture value
  int moistureLevel = analogRead(MOISTURE_PIN);

  // Read light intensity from LDR
  int lightLevel = analogRead(LDR_PIN);

  // Display data on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(temperature);   // Display temperature value
  
  lcd.setCursor(8, 0);      // Move to second half of first row
  lcd.print(humidity);      // Display humidity value
  
  lcd.setCursor(0, 1);      // Move to second row
  lcd.print(moistureLevel); // Display moisture level value
  
  lcd.setCursor(8, 1);      // Move to second half of second row
  lcd.print(lightLevel);    // Display light intensity value

  // Print to Serial Monitor for debugging
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, \nHumidity: ");
  Serial.print(humidity);
  Serial.print(" %, \nMoisture: ");
  Serial.print(moistureLevel);
  Serial.print(", \nLight: ");
  Serial.println(lightLevel);
  Serial.print("\n");

  // Check if moisture is below threshold and activate buzzer
  if (moistureLevel < MOISTURE_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);  // Turn on the buzzer
    lcd.setCursor(0, 1);
    lcd.print("Watered");
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // Turn off the buzzer
    delay(100);
    digitalWrite(BUZZER_PIN,HIGH);
    delay(1000);
  }
  
  // Small delay to avoid rapid updates
  delay(1000);  // Delay for 1 second before reading again
}
