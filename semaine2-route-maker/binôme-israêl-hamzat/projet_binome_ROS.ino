#include <LiquidCrystal.h>
#include <Servo.h>

/* Pin definitions */
#define LDR A0
#define LED_R 4
#define LED_G 5
#define POT A1
#define SOUNDER 6
#define SERVO 3
#define E 7
#define RS 8
#define D4 9
#define D5 10
#define D6 11
#define D7 12

// 
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
Servo servo;

// External functions declaration
void printLCD(int state, int value = 0);
void RGBState(int state);
void sounderActive(unsigned long duration);

// Code variables
int previousThreshold;
unsigned long currentDuration;
bool isSet = false, servoActive = false;
bool sounderState = LOW; 

void setup() {
  pinMode(LDR, INPUT);
  pinMode(POT, INPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(SOUNDER, OUTPUT);
  servo.attach(SERVO);

  // LCD initialization
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Activation en");
  lcd.setCursor(0, 1);
  lcd.print("cours...");
  delay(3000);
  lcd.clear();

  previousThreshold = analogRead(POT);

  // put your setup code here, to run once:

}

void loop() {
  // LDR and Potentiometer values read
  int LDRValue = analogRead(LDR);
  int threshold = analogRead(POT);

  if (abs(threshold - previousThreshold) > 3) {
    previousThreshold = threshold;
    printLCD(0, threshold);
    isSet = false;
  }
  else {
    isSet = true;
  }

  // Main code for system activation
  if (LDRValue > threshold && !servoActive && isSet) {
    printLCD(1);
    digitalWrite(SOUNDER, LOW);
    RGBState(1);
    servo.write(0);
    delay(170);
    servoActive = true;
    sounderState = LOW;
  }
  else if (LDRValue <= threshold && isSet) {
    if (servoActive) {
      printLCD(2);
      RGBState(0);
      servo.write(90);
      delay(170);
      servoActive = false;
      currentDuration = millis();
      sounderState = HIGH;
      digitalWrite(SOUNDER, sounderState);
    }
    sounderActive(300);
  }
}


/* Externals functions */

// Printing on LCD funtion
void printLCD(int state, int value) {
  lcd.clear();
  if (state == 1) {
    lcd.print("Aucune");
    lcd.setCursor(0, 1);
    lcd.print("anomalie...");
  }
  else if (state == 2) {
    
    lcd.print("Changement");
    lcd.setCursor(0, 1);
    lcd.print("observe...");
  }
  else {
    lcd.print("Reglage seuil :");
    lcd.setCursor(0, 1);
    lcd.print(value);
  }
}

// RGB state settings
void RGBState (int state) {
  bool ledState = (state == 1)? HIGH : LOW;
  digitalWrite(LED_G, ledState);
  digitalWrite(LED_R, !ledState);
}

// Sounder alarm without delay
void sounderActive(unsigned long duration) {
  if (millis() - currentDuration >= duration) {
    sounderState = !sounderState;
    digitalWrite(SOUNDER, sounderState);
    currentDuration = millis();
  }
}