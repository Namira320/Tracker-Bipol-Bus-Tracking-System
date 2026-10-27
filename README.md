# Tracker-Bipol: IoT-Based Campus Bus Tracking System

An IoT-based real-time bus tracking prototype designed to modernize campus internal transportation. This system utilizes **NodeMCU ESP8266** microcontrollers, **GPS Neo-6M** modules, **MQTT protocol**, and an **I2C 16x2 LCD** to track bus locations and display real-time arrival info at campus bus stops.

---

## 🚀 Features
- **Real-Time GPS Tracking:** Captures live latitude and longitude coordinates using the GPS Neo-6M module.
- **MQTT Protocol Integration:** Lightweight and efficient messaging using a public MQTT broker (`broker.hivemq.com`) to transmit location data from the bus node to the stop display node.
- **Smart Stop Detection:** Automatically calculates distance and detects nearby stops using latitude/longitude thresholds.
- **LCD I2C Display:** Shows real-time bus status and current stop information directly on a digital screen at the bus stop.

---

## 🛠️ Hardware & Components
| Component | Quantity | Description |
| :--- | :---: | :--- |
| **NodeMCU ESP8266** | 2 | Microcontroller units (one for the bus transmitter, one for the stop receiver) |
| **GPS Neo-6M Module** | 1 | Satellite positioning module with antenna |
| **I2C 16x2 LCD** | 1 | Display screen for bus stop notifications |
| **18650 Battery + Casing** | 2 | Power supply for the mobile bus unit |
| **Step-Up Converter** | 2 | Voltage booster |
| **Jumper Wires & USB Cables** | - | Circuit connections and power delivery |

---

## 📁 Repository Structure
```text
Tracker-Bipol-Bus-Tracking-System/
├── Bus_Node_Code/         # Code for the mobile bus transmitter (ESP8266 + GPS)
├── Stop_Node_Code/        # Code for the bus stop receiver (ESP8266 + I2C LCD)
└── README.md              # Project documentation

---

⚙️ How It Works
Bus Transmitter Node:

The GPS module reads coordinates from satellite signals.

The ESP8266 processes the data and publishes it as a JSON payload via MQTT to a specified topic (exploreCscKel3/bus).

Bus Stop Receiver Node:

The ESP8266 subscribes to the same MQTT topic.

Upon receiving incoming location data, it parses the JSON, checks proximity to predefined campus stops, and displays the status on the I2C LCD screen.
