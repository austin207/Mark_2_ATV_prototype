#include <HardwareSerial.h>
// Define new motor control pins for L298N (4 Motors)
#define ENA 25   // New PWM Pin for Motor A (left side motor speed)
#define ENB 26   // New PWM Pin for Motor B (right side motor speed)
#define IN1 27   // New Motor A control pin 1 (left motor)
#define IN2 14   // New Motor A control pin 2 (left motor)
#define IN3 12   // New Motor B control pin 1 (right motor)
#define IN4 13   // New Motor B control pin 2 (right motor)

// Bluetooth HC-05 communication pins (Hardware Serial)
#define BT_RX_PIN 16  // New RX pin of Bluetooth module (connected to TX2 of ESP32)
#define BT_TX_PIN 17  // New TX pin of Bluetooth module (connected to RX2 of ESP32)

// Ready to Run indicator
#define ready_LED 2   // New LED pin to indicate readiness

// Use Hardware Serial 2 for Bluetooth communication
HardwareSerial BTSerial(2);

int speedValue = 255;  // Default max speed

bool connectionEstablished = false;  // Debugging connectivity issues

void setup() {
  // Serial monitor for debugging
  Serial.begin(9600);  // Start serial communication for debugging

  // Initialize Hardware Serial 2 for Bluetooth
  BTSerial.begin(9600, SERIAL_8N1, BT_RX_PIN, BT_TX_PIN);

  // Initialize motor control pins as outputs
  pinMode(ENA, OUTPUT);  
  pinMode(ENB, OUTPUT);  
  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);  
  pinMode(IN3, OUTPUT);  
  pinMode(IN4, OUTPUT);  

  // LED indicator setup
  pinMode(ready_LED, OUTPUT);    
  digitalWrite(ready_LED, LOW);  // Initially, LED is off (no connection)

  // Set initial motor speed to 0 (stopped)
  analogWrite(ENA, 0);
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
    int inputSpeed = BTSerial.parseInt();  // Extract numerical speed information (0 - 255)

    if (inputSpeed >= 0 && inputSpeed <= 255) {
      speedValue = inputSpeed;  // Override speed with user input
    } else {
      Serial.println("Invalid Speed value! Keeping default speed.");
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
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

// Move the car backward
void moveBackward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

// Turn the car left
void turnLeft(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed / 2);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

// Turn the car right
void turnRight(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed / 2);
}

// Stop all motors
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}
