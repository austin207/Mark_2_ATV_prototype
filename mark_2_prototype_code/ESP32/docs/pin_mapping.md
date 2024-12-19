# **Pin Mapping for (ESP32) Bluetooth-Controlled Car**

## **Motor Driver (L298N) - 4 Motors**
| Pin Name | ESP32 Pin | Function                      |
|----------|-----------|-------------------------------|
| ENA      | 5         | PWM pin for Motor A (left side motor speed) |
| ENB      | 3         | PWM pin for Motor B (right side motor speed) |
| IN1      | 8         | Control pin 1 for Motor A (left motor)       |
| IN2      | 9         | Control pin 2 for Motor A (left motor)       |
| IN3      | 12        | Control pin 1 for Motor B (right motor)      |
| IN4      | 13        | Control pin 2 for Motor B (right motor)      |

---

## **Bluetooth HC-05 Communication**
| Pin Name   | ESP32 Pin | Function                                   |
|------------|-----------|-------------------------------------------|
| BT_RX_PIN  | 16        | RX pin of Bluetooth module (connected to TX2 of ESP32) |
| BT_TX_PIN  | 17        | TX pin of Bluetooth module (connected to RX2 of ESP32) |

---

## **Ready-to-Run Indicator**
| Pin Name  | ESP32 Pin | Function                                   |
|-----------|-----------|-------------------------------------------|
| ready_LED | 7         | LED indicator (lights up when connection is established) |

---

## **Speed and Commands**
| Parameter      | Values        | Description                                      |
|----------------|---------------|--------------------------------------------------|
| `speedValue`   | 0–255         | Controls the speed of the motors (default: 255) |
| `command`      | F, B, L, R, S | F = Forward, B = Backward, L = Left, R = Right, S = Stop |
