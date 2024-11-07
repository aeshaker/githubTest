#include <LiquidCrystal.h>

// Define pin numbers for LCD display
const int lcdRSPin = 2;
const int lcdEPin = 3;
const int lcdD4Pin = 4;
const int lcdD5Pin = 5;
const int lcdD6Pin = 6;
const int lcdD7Pin = 7;

// Define pin numbers for motor control and microstepping
const int dirPin = 8;
const int stepPin = 9;
const int M0 = 11;
const int M1 = 12;
const int M2 = 13;

// Define pin numbers for buttons and limit switches
const int forwardButton = 14;
const int backwardButton = 15;
const int limit1 = 18;
const int limit2 = 19;


// Motor step settings
//const int stepsPerRevolution = 1600;   // 1/8 microstepping, so 1600 steps per full rotation
const unsigned long stepDelay = 6250;  // Adjusted delay for 20 seconds per revolution




// Direction definitions
#define CLOCKWISE 1
#define COUNTERCLOCKWISE 0

// Initialize variables
LiquidCrystal lcd(lcdRSPin, lcdEPin, lcdD4Pin, lcdD5Pin, lcdD6Pin, lcdD7Pin);
int motorDirection = CLOCKWISE;   // Default direction
bool motorStopped = true;         // Start with motor stopped
int position = 900;               // Initial position in nm

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stepper V1.0");
  delay(3000);

  // Set pin modes for motor driver, buttons, and limit switches
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  pinMode(forwardButton, INPUT_PULLUP);
  pinMode(backwardButton, INPUT_PULLUP);
  pinMode(limit1, INPUT_PULLUP);
  pinMode(limit2, INPUT_PULLUP);

  // Set microstepping to 1/8 (M0 = HIGH, M1 = HIGH, M2 = LOW)
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Ready");
}

void loop() {
  // Check forward button to set direction
  if (digitalRead(forwardButton) == LOW) {
    motorDirection = CLOCKWISE;
    motorStopped = false;
    delay(150);    // Debounce delay
    lcd.clear();   // Clear LCD when direction changes
  }

  // Check backward button to set direction
  else if (digitalRead(backwardButton) == LOW) {
    motorDirection = COUNTERCLOCKWISE;
    motorStopped = false;
    delay(150);    // Debounce delay
    lcd.clear();   // Clear LCD when direction changes
  }

  // Check limit switches to stop motor
  if (digitalRead(limit1) == LOW || digitalRead(limit2) == LOW) {
    motorStopped = true;
    delay(150);       // Debounce delay for limit switch
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stopped");
  }

  // Motor stepping logic (only if motor is not stopped)
  if (!motorStopped) {
    digitalWrite(dirPin, motorDirection);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);

    // Update position based on motor direction
    if (motorDirection == CLOCKWISE) {
      position += 10;
    } else {
      position -= 10;
    }
  }

  // LCD Updates for Motor Status and Position
  lcd.setCursor(0, 0);
  if (motorStopped) {
    lcd.print("Stopped       ");
  } else if (motorDirection == CLOCKWISE) {
    lcd.print("Forward       ");
  } else {
    lcd.print("Backward      ");
  }

  lcd.setCursor(0, 1);
  lcd.print("Position:");
  lcd.print(position);
  lcd.print(" nm ");  // Extra spaces to clear old characters

  delay(1000);  // Refresh display every second
}

