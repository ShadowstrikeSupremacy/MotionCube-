#include <Wire.h>
#include <MPU6050.h>
#include <LiquidCrystal_I2C.h>

#define SDA_PIN 32
#define SCL_PIN 33

MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 8, 2);  // Initialize LCD at I2C address 0x27, 8x2 display

float roll, pitch, rollF = 0, pitchF = 0;
int16_t ax, ay, az;  // Accelerometer data

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);     // Start I2C on custom pins before using LCD or MPU
  lcd.init();                       // Initialize the LCD
  lcd.backlight();                  
  
  delay(1000);
  Serial.println("ESP32 STARTED");

  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed.");
    while (1);
  }
  
  // Offset Calibration (adjust if needed)
  mpu.setYAccelOffset(0);
  mpu.setZAccelOffset(0);
}

void loop() {
  // Read accelerometer data
  mpu.getAcceleration(&ax, &ay, &az);
  
  // Convert raw values to g
  float X_out = ax / 16384.0;
  float Y_out = ay / 16384.0;
  float Z_out = az / 16384.0;

  // Calculate roll and pitch
  roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI - 54;
  pitch = atan(-X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;

  // Low-pass filter
  rollF = 0.94 * rollF + 0.06 * roll;
  pitchF = 0.94 * pitchF + 0.06 * pitch;

  // Serial output for Processing
  Serial.print(rollF);
  Serial.print("/");
  Serial.println(pitchF);

  // LCD output
  lcd.setCursor(0, 0);
  lcd.print("Motion");

  lcd.setCursor(0, 1);
  lcd.print("Tracker");

  delay(10);  // Small delay for readability
}