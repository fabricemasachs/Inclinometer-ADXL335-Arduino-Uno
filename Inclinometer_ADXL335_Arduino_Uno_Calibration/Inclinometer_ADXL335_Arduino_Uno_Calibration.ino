//
// Sketch for the Inclinometer ADXL335 Arduino Uno Calibration 
// Copyright (c) 2015 Fabrice Masachs, OH6FKO
// Released under the MIT License
//
// Uses the Arduino Serial Monitor to trigger calibrations
// Runs a calibration each time it receives a "c" from the USB serial 
// The calibration should be performed with the accelerometer rotated to min and max positions for each Axis
// - calibrations for the same Axis can be repeated
// - Results for each calibration are retained through the entire calibration procedure
// - Mix/Max Values of 512 indicate that an axis has not been calibrated.

const int xInput = A5;
const int yInput = A4;
const int zInput = A3;

// Raw Ranges:
// initialize to mid-range and allow calibration to
// find the minimum and maximum for each axis
int xRawMin = 512;
int xRawMax = 512;
 
int yRawMin = 512;
int yRawMax = 512;
 
int zRawMin = 512;
int zRawMax = 512;
 
// Take multiple samples to reduce noise
const int sampleSize = 10;
 
void setup() {
  analogReference(EXTERNAL); //Connect 3.3v to AREF
  
  Serial.begin(38400);
}
 
void loop() {
  char startByte = '0';
  
  if (Serial.available() > 0) {
    // read the byte
    startByte = Serial.read();
    Serial.print("Begin Calibration - received ");
    Serial.println(startByte);
   }
  
  if ((int)startByte == 'c') {
    delay(250);
    int xRaw = ReadAxis(xInput);
    int yRaw = ReadAxis(yInput);
    int zRaw = ReadAxis(zInput);
  
    AutoCalibrate(xRaw, yRaw, zRaw);

    Serial.print("Raw Ranges: X: ");
    Serial.print(xRawMin);
    Serial.print("-");
    Serial.print(xRawMax);
    
    Serial.print(", Y: ");
    Serial.print(yRawMin);
    Serial.print("-");
    Serial.print(yRawMax);
    
    Serial.print(", Z: ");
    Serial.print(zRawMin);
    Serial.print("-");
    Serial.print(zRawMax);
    Serial.println();
    Serial.print(xRaw);
    Serial.print(", ");
    Serial.print(yRaw);
    Serial.print(", ");
    Serial.print(zRaw);
    
    // Convert raw values to 'milli-Gs"
    long xScaled = map(xRaw, xRawMin, xRawMax, -1000, 1000);
    long yScaled = map(yRaw, yRawMin, yRawMax, -1000, 1000);
    long zScaled = map(zRaw, zRawMin, zRawMax, -1000, 1000);
  
    // re-scale to fractional Gs
    float xAccel = xScaled / 1000.0;
    float yAccel = yScaled / 1000.0;
    float zAccel = zScaled / 1000.0;
  
    Serial.print(" :: ");
    Serial.print(xAccel);
    Serial.print("G, ");
    Serial.print(yAccel);
    Serial.print("G, ");
    Serial.print(zAccel);
    Serial.println("G");
    
    //reset the startByte
    startByte = '0';
  }
}
 
//
// Read "sampleSize" samples and report the average
//
int ReadAxis(int axisPin) {
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++) {
    reading += analogRead(axisPin);
  }
  
  return reading/sampleSize;
}
 
//
// Find the extreme raw readings from each axis
//
void AutoCalibrate(int xRaw, int yRaw, int zRaw) {
  Serial.println("Calibrate");
  if (xRaw < xRawMin) {
    xRawMin = xRaw;
  }
  if (xRaw > xRawMax) {
    xRawMax = xRaw;
  }
  
  if (yRaw < yRawMin) {
    yRawMin = yRaw;
  }
  if (yRaw > yRawMax) {
    yRawMax = yRaw;
  }
 
  if (zRaw < zRawMin) {
    zRawMin = zRaw;
  }
  if (zRaw > zRawMax) {
    zRawMax = zRaw;
  }
}
