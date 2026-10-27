Tracker BIPOL (Bus Tracking IoT System)

An Internet of Things (IoT) based real-time campus bus tracking system developed as a final project for the Computer Student Club (KSM CSC), Politeknik Negeri Jakarta.

🚌 Overview

Tracker BIPOL is a low-cost, smart campus transportation solution designed to resolve operational uncertainties regarding campus bus locations and schedules. The system utilizes a dual-node architecture connected wirelessly via the MQTT protocol, transmitting real-time GPS coordinate data from a moving vehicle unit to a digital bus stop display unit.

🛠️ System Architecture & Components

The system is divided into two main subsystems:

Bus/Vehicle Unit (Transmitter): Powered by an ESP8266 microcontroller paired with a NEO-6M GPS module to gather coordinates and publish them to an MQTT broker.

Bus Stop Unit (Receiver): Powered by an ESP8266 microcontroller subscribed to the MQTT broker, which processes the coordinates, detects the closest campus bus stop, and renders the information onto an I2C 16x2 LCD screen.

Hardware Requirements

NodeMCU ESP8266 (x2)

NEO-6M GPS Module with Antenna (x1)

I2C 16x2 LCD Display (x1)

18650 Batteries + Casing (x2)

Step-Up Converter (x2)

USB to Micro USB Cables (x2)

Female-to-Female Jumper Wires (As needed)

📂 Repository Structure

├── bus_tracker_sender/
│   └── bus_tracker_sender.ino       # Source code for the bus/vehicle transmitter unit
├── bus_stop_receiver/
│   └── bus_stop_receiver.ino        # Source code for the bus stop display receiver unit
└── README.md


⚙️ Installation & Setup

Prerequisites

Make sure you have the following installed:

Arduino IDE

Required Arduino Libraries:

ESP8266WiFi

PubSubClient (by Nick O'Leary for MQTT)

SoftwareSerial

TinyGPS++ (by Mikal Hart)

LiquidCrystal_I2C

ArduinoJson

Configuration

Open bus_tracker_sender.ino and update your Wi-Fi credentials and MQTT broker settings:

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "broker.hivemq.com";


Open bus_stop_receiver.ino and update the matching Wi-Fi and MQTT configurations, along with your defined campus stop coordinates if necessary.
