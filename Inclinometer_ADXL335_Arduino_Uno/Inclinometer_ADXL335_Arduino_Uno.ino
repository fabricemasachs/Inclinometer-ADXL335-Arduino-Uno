//
// Sketch for the Inclinometer ADXL335 Arduino Uno 
// Copyright (c) 2015 Fabrice Masachs, OH6FKO
// Released under the MIT License
//

#include <LiquidCrystal.h> //Load Liquid Crystal Library

LiquidCrystal lcd(8,9,10,11,12,13); //Create Liquid Crystal Object called lcd

//Analog read pins
const int xInput = A5;
const int yInput = A4;
const int zInput = A3;

//The minimum and maximum values for each axis 
//measured from calibration
const int xMin = 409; // (-1 G)
const int xMax = 617; // (+1 G)

const int yMin = 404; // (-1 G)
const int yMax = 612; // (+1 G)

const int zMin = 426; // (-1 G)
const int zMax = 632; // (+1 G)

//To hold the caculated values
double x;
double y;
double z;

//ASCII character for degree symbol
const int degree = 223;

// Make custom characters: 
byte leftArrow[8] = {
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
  B00000
};

byte rightArrow[8] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000
};

byte upArrow[8] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000
};

byte downArrow[8] = {
  B00000,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000
};

void setup() {
  analogReference(EXTERNAL); //Connect 3.3v to AREF

  Serial.begin(38400); 

  lcd.begin(16,2); //Tell Arduino to start your 16 column 2 row LCD

  splashScreen();
}

void splashScreen() {    
  lcd.setCursor(0,0); //Set LCD cursor to upper left corner, column 0, row 0
  lcd.print("  INCLINOMETER  "); //Print message on first row
  lcd.setCursor(0,1);
  lcd.print("  WITH ADXL335  "); //Print message on second row

  //Wait for splash screen
  delay(3000);

  lcd.clear();
}

void loop() {
  //Read the analog values from the accelerometer
  int xRead = analogRead(xInput);
  int yRead = analogRead(yInput);
  int zRead = analogRead(zInput);
  
  //convert read values to degrees -90 to 90 - Needed for atan2
  int xAng = map(xRead, xMin, xMax, -90, 90);
  int yAng = map(yRead, yMin, yMax, -90, 90);
  int zAng = map(zRead, zMin, zMax, -90, 90);
  
  //Caculate 360deg values like so: atan2(-yAng, -zAng)
  //atan2 outputs the value of -π to π (radians)
  //We are then converting the radians to degrees
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  if (x > 180) {
    x = x - 360;
  }
  
  if (y > 180) {
    y = y - 360;
  }

  int xToInt = (int)x;
  String xToIntString = String(xToInt);

  Serial.print("Roll: ");
  Serial.print(xToIntString);
  
  if ((xToIntString == "0") || (xToIntString == "1") || (xToIntString == "-1")) {
    Serial.print(" degree");  
  } else {
    Serial.print(" degrees");
  }
  
  int yToInt = (int)y;
  String yToIntString = String(yToInt);

  Serial.print(" | Pitch: ");
  Serial.print(yToIntString);

  if ((yToIntString == "0") || (yToIntString == "1") || (yToIntString == "-1")) {
    Serial.println(" degree");  
  } else {
    Serial.println(" degrees");
  }

  lcd.createChar(1, leftArrow);
  lcd.createChar(2, rightArrow);
  
  if (xToIntString.substring(0,1) == "-") {
    xToIntString.remove(0,1);
    xToIntString = "\1" + xToIntString;
  } else if ((xToIntString.substring(0,1) != "-") & (xToIntString.substring(0,1) != "0")) {
    xToIntString = "\2" + xToIntString;
  } else if (xToIntString.substring(0,1) == "0") {
    xToIntString = "0";
  }

  int xToIntStringLength = xToIntString.length() + 1;
  
  if (xToIntStringLength == 2) {
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Roll:         " + xToIntString);
    lcd.write(degree);
  } else if (xToIntStringLength == 3) {
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Roll:        " + xToIntString);
    lcd.write(degree);
  } else if (xToIntStringLength == 4) {
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Roll:       " + xToIntString);
    lcd.write(degree);
  } else if (xToIntStringLength == 5) {
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Roll:      " + xToIntString);
    lcd.write(degree);
  }

  lcd.createChar(3, upArrow);
  lcd.createChar(4, downArrow);
  
  if (yToIntString.substring(0,1) == "-") {
    yToIntString.remove(0,1);
    yToIntString = "\3" + yToIntString;
  } else if ((yToIntString.substring(0,1) != "-") & (yToIntString.substring(0,1) != "0")) {
    yToIntString = "\4" + yToIntString;
  } else if (yToIntString.substring(0,1) == "0") {
    yToIntString = "0";
  }
  
  int yToIntStringLength = yToIntString.length() + 1;
  
  if (yToIntStringLength == 2) {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Pitch:        " + yToIntString);
    lcd.write(degree);
  } else if (yToIntStringLength == 3) {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Pitch:       " + yToIntString);
    lcd.write(degree);
  } else if (yToIntStringLength == 4) {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Pitch:      " + yToIntString);
    lcd.write(degree);
  } else if (yToIntStringLength == 5) {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Pitch:     " + yToIntString);
    lcd.write(degree);
  }

  delay(250);
  
}
