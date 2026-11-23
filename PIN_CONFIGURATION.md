# Hardware Connection Information (Table II and Pin Map)

This document lists the components of the prototype and their connections to the ESP32 (ESP32-WROOM-32 DevKit). The pin numbers match the firmware in the `code` folder. The wiring diagram is `circuit/circuit_diagram.png`.

## 1. ESP32 Pin Configuration

| Component | Model / Specification | ESP32 GPIO | Signal type / Notes |
|---|---|---|---|
| Ultrasonic sensor | HC-SR04 | Trig: GPIO 5, Echo: GPIO 18 | Digital pulse output / input |
| Gas and smoke sensor | MQ-2 module | GPIO 34 (ADC1) | Analog input, 12-bit ADC (0-4095) |
| Flame sensor | HW-072 | GPIO 14 | Digital input (INPUT_PULLUP), active LOW |
| IR intrusion sensor | FC-51 | GPIO 13 | Digital input (INPUT_PULLUP), active LOW |
| Rain sensor | Rain sensor module | GPIO 27 | Digital input (INPUT_PULLUP), LOW = rain |
| Door servo | SG90 | GPIO 2 | PWM output |
| Clothesline servo | SG90 | GPIO 4 | PWM output |
| Buzzer | Active piezo buzzer | GPIO 25 | LEDC output (2 kHz tone) |
| GSM module | SIM800L | GPIO 16 (ESP32 RX), GPIO 17 (ESP32 TX) | UART (Serial1), 9600 baud |

## 2. SIM800L UART Connections

| SIM800L pin | ESP32 pin |
|---|---|
| TXD | GPIO 16 (ESP32 RX) |
| RXD | GPIO 17 (ESP32 TX) |
| GND | Common GND |
| VCC | Separate 3.7 V battery (see Section 4) |

## 3. Rain Sensor and Servo Connections

| Component | Connection |
|---|---|
| Rain sensor DO | GPIO 27 |
| Rain sensor VCC / GND | Supply according to the module specification / common GND |
| Clothesline servo signal | GPIO 4 |
| Door servo signal | GPIO 2 |
| Servo VCC / GND | 5 V supply / common GND |

## 4. Power Supply

| Item | Description |
|---|---|
| Main supply | Two 3.7 V 18650 cells in series (7.4 V) |
| Regulation | LM2596 buck converter, set to 5 V |
| ESP32 3.3 V | Provided by the on-board regulator of the ESP32 DevKit |
| SIM800L supply | Third 3.7 V 18650 cell, directly connected to the module |
| SIM800L capacitor | 470 uF capacitor across the SIM800L supply terminals, in parallel with the cell |
| Charging | External Li-ion charger; there is no charging circuit in the prototype |
| Ground | The cell, the capacitor and the SIM800L share one ground, which is connected to the common ground of the main circuit |

The SIM800L can draw current peaks of about 2 A during transmission, so it has its own cell and must not be powered from the ESP32.

## 5. Important Electrical Notes

1. The SIM800L and the ESP32 must share a common ground.
2. The MQ-2 analog output and the SIM800L UART lines are connected directly to the ESP32 without a voltage divider or level shifter. This worked in the prototype, but the MQ-2 output can exceed 3.3 V, so a divider is recommended in a final design.
3. The HC-SR04 Echo output is a 5 V signal. Use a voltage divider or level shifter before the ESP32 pin in a final design.
4. GPIO 34 is an input-only pin.
5. GPIO 2 is a boot-strapping pin on the ESP32. If the board does not boot reliably, check this connection first.
6. Check the voltage and current requirements of each component before powering the circuit.
7. Verify all connections against your own hardware before operating the system.

## 6. Notes

- The rain sensor only moves the clothesline servo. The firmware sends no Blynk or SMS alert for rain.
- The ultrasonic sensor only moves the door servo. The firmware sends no alert for it.

