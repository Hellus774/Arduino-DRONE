#include <Wire.h>
#include <math.h>

const int MPU_addr = 0x68; // MPU-6050 I2C address

int16_t accelerometer_x, accelerometer_y, accelerometer_z;

// Threshold for movement detection
const int movementThreshold = 1000;

void setup() {
  Wire.begin();        // Initialize I2C communication
  Serial.begin(9600);  // Initialize serial communication
  while (!Serial);     // Wait for serial monitor to open
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);    // PWR_MGMT_1 register
  Wire.write(0);       // Set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void loop() {
  // Read accelerometer data
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);    // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true); // request a total of 6 registers

  accelerometer_x = Wire.read() << 8 | Wire.read();
  accelerometer_y = Wire.read() << 8 | Wire.read();
  accelerometer_z = Wire.read() << 8 | Wire.read();

  // Calculate the angle of inclination in each axis
  float angle_x = atan2(accelerometer_y, sqrt(accelerometer_x * accelerometer_x + accelerometer_z * accelerometer_z)) * 180 / PI;
  float angle_y = atan2(-accelerometer_x, sqrt(accelerometer_y * accelerometer_y + accelerometer_z * accelerometer_z)) * 180 / PI;

  // Check for movement in left, right, front, and back directions
  if (abs(accelerometer_x) > movementThreshold) {
    if (accelerometer_x > 0) {
      Serial.print("Right ");
    } else {
      Serial.print("Left ");
    }
    Serial.print(abs(angle_x));
    Serial.println(" degrees");
  } else if (abs(accelerometer_y) > movementThreshold) {
    if (accelerometer_y > 0) {
      Serial.print("Backward ");
    } else {
      Serial.print("Forward ");
    }
    Serial.print(abs(angle_y));
    Serial.println(" degrees");
  } else {
    Serial.println("No significant movement");
  }

  delay(500); // Delay for stability
}
