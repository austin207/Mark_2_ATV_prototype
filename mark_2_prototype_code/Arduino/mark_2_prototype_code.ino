#include <SoftwareSerial.h>  // for custom rx and tx pins

// Define motor control pins for L298N (4 Motors)
#define ENA 5   // PWM Pin for Motor A (left side motor speed)
#define ENB 3   // PWM Pin for Motor B (right side motor speed)
#define IN1 8   // Motor A control pin 1 (left motor)
#define IN2 9   // Motor A control pin 2 (left motor)
#define IN3 12  // Motor B control pin 1 (right motor)
#define IN4 13  // Motor B control pin 2 (right motor)

// Bluetooth HC-05 communication pins
#define BT_RX_PIN 10  // RX pin of Bluetooth module (connected to TX pin of Arduino) it is defined for an easier implementation in the code
#define BT_TX_PIN 11  // TX pin of Bluetooth module (connected to RX pin of Arduino) it is defined for an easier implementation in the code

// Ready to Run indicator
#define ready_LED 7  // LED to indicate that the car is ready for running

SoftwareSerial BTSerial(BT_RX_PIN, BT_TX_PIN);  // RX, TX pins for Bluetooth communication from lines 12, 13

int speedValue = 255;  //Default max speed (By initializing it to 255, you ensure that if no speed input is received the car defaults to running at full speed.)

bool connectionEstablished = false;  //setting a parameter to debug connectivity issues

void setup() {
  // Serial monitor for debugging
  Serial.begin(9600);  // Start serial communication with Bluetooth module and for debugging

  BTSerial.begin(9600);  // Bluetooth communication speed (usually 9600 bps => bits per second)

  // Initialize motor control pins as outputs from lines 4 to 9 we assigned keywords for pins to improve code readability
  pinMode(ENA, OUTPUT);  //5
  pinMode(ENB, OUTPUT);  //3
  pinMode(IN1, OUTPUT);  //8
  pinMode(IN2, OUTPUT);  //9
  pinMode(IN3, OUTPUT);  //12
  pinMode(IN4, OUTPUT);  //13

  // LED indicator setup
  pinMode(ready_LED, OUTPUT);    //7
  digitalWrite(ready_LED, LOW);  // Initially, LED is off (no connection)

  // Set initial motor speed to 0 (stopped)
  analogWrite(ENA, 0);  //even though it is a digital pin analogewrite function can be used to adjust the duty cycle of a pwm pin (the frequency of rapid changing between high and low) which is preferred for controlling a motor since it takes average velocity of the changes in levels
  analogWrite(ENB, 0);
}

void loop() {
  // Connectivity debugging statements
  if (BTSerial.available()) {
    if (!connectionEstablished) {
      Serial.println("Bluetooth Connection Established!!");
      connectionEstablished = true;
      digitalWrite(ready_LED, HIGH);
    }
  }


  // Check if data is available from Bluetooth
  if (BTSerial.available() > 0) {
    char command = BTSerial.read();  // Read the incoming character from Bluetooth (F, B, L, R, S)
    delay(10);
    int inputSpeed = BTSerial.parseInt();  // to extract numerical speed information from the command (0 - 255)

    if (inputSpeed >= 0 && inputSpeed <= 255) {
      speedValue = inputSpeed;  //Speed overriding function to set desired speed by user
    } else {
      Serial.println("Invalid Speed value! keeping default Speed.");
    }

    // Debugging output to serial monitor
    Serial.print("Received command: ");
    Serial.println(command);

    // Control motor based on the received command
    switch (command) {
      case 'F':  // Move Forward
        moveForward(speedValue);
        break;
      case 'B':  // Move Backward
        moveBackward(speedValue);
        break;
      case 'L':  // Turn Left
        turnLeft(speedValue);
        break;
      case 'R':  // Turn Right
        turnRight(speedValue);
        break;
      case 'S':  // Stop
        stopMotors();
        break;
      default:
        stopMotors();  // Stop the car for any invalid command
        break;
    }
  }
}

// Move the car forward
void moveForward(int speed) {
  // Forward for left motors (Motor A)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);  // Full speed for left motors

  // Forward for right motors (Motor B)
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);  // Full speed for right motors
}

// Move the car backward
void moveBackward(int speed) {
  // Backward for left motors (Motor A)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);  // Full speed for left motors

  // Backward for right motors (Motor B)
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);  // Full speed for right motors
}

// Turn the car left
void turnLeft(int speed) {
  // Stop Left motors and move Right motors forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed / 2);  // Reduced speed for left motors

  // Stop right motors
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);  // Full speed for right motors
}

// Turn the car right
void turnRight(int speed) {
  // Stop Right motors and move Left motors forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);  // Full speed for Left motors

  // Stop left motors
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed / 2);  // Reduced speed for right motors
}

// Stop all motors
void stopMotors() {
  // Stop left motors
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);  // No speed for left motors

  // Stop right motors
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);  // No speed for right motors
}
