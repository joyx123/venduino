#include <Servo.h>

const int buttonPin = 4;   // Pin connected to the pushbutton
const int servoPin = 10;   // Pin connected to the servo
const int interruptPin = 2;  // Pin connected to the interrupt

Servo myServo;

int buttonState = 0;       // Variable to store the pushbutton status
int lastButtonState = HIGH; // Variable to store the last button state
int servoPosition = 0;     // Variable to store the servo position
int angleWhenPressed = 90; // Angle to move the servo to when button is pressed

unsigned long lastDebounceTime = 0;  // The last time the button state was toggled
unsigned long debounceDelay = 50;    // Debounce time

int iCount = 0;            // Impulse count
bool servoMoved = false;    // Flag to track if the servo has moved

// Interrupt handler for incoming impulses
void iIncoming() {
  iCount += 10;
  Serial.print("Impulse detected. iCount: ");
  Serial.println(iCount);
}

void setup() {
  // Debugging to confirm the setup is working
  Serial.begin(9600);
  Serial.println("Setup started");

  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor
  myServo.attach(servoPin);         // Attach the servo to the specified pin
  myServo.write(0);                 // Set initial position of the servo to 0 degrees

  // Use a different pin for the interrupt (like pin 2)
  attachInterrupt(digitalPinToInterrupt(interruptPin), iIncoming, RISING); // Set up interrupt on pin 2

  Serial.println("Setup complete");
}

void loop() {
  int reading = digitalRead(buttonPin); // Read the state of the button

  // Debugging to check if button press is detected
  Serial.print("Button state: ");
  Serial.println(reading);

  // Check for button state changes with debouncing
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Reset debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading; // Update button state

      if (buttonState == LOW) { // Button is pressed
        Serial.println("Button pressed");

        // Only move the servo if an impulse has been detected and it hasn't moved yet
        if (iCount > 100 && !servoMoved) {
          servoPosition = angleWhenPressed; // Move the servo to the angle when button is pressed
          myServo.write(servoPosition);
          delay(500); // Wait for the servo to reach the position
          servoMoved = true;  // Set flag to indicate the servo has moved

          // Reset impulse count to zero after moving the servo
          iCount = 0;
          Serial.println("Impulse count reset to zero.");
        } else if (iCount <= 100) {
          Serial.println("Insufficient impulses. Servo not moving.");
        }
      } else { // Button is released
        Serial.println("Button released");
        if (servoPosition != 0) {
          servoPosition = 0; // Move the servo back to the initial position
          myServo.write(servoPosition);
          delay(500); // Wait for the servo to reach the position
          servoMoved = false;  // Reset the flag
          Serial.println("Servo reset to position 0.");
        }
      }
    }
  }

  lastButtonState = reading; // Save the current state as the last state

  // Ensure the servo stays at 0 position if iCount is 0 and servo hasn't moved
  if (iCount == 0 && servoPosition != 0 && servoMoved) {
    servoPosition = 0;
    myServo.write(servoPosition); // Reset to 0 when no impulses and servo has moved
    servoMoved = false;  // Reset the flag
    Serial.println("Servo forced to position 0 due to zero impulse count.");
  }

  // Debugging - Log the impulse count and servo state in every loop iteration
  Serial.print("Current impulse count: ");
  Serial.println(iCount);
  Serial.print("Servo position: ");
  Serial.println(servoPosition);
  Serial.print("Servo moved: ");
  Serial.println(servoMoved ? "Yes" : "No");
}
