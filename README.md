# IoT-Based-Inventory-Management
This project designs and implements a cost effective smart shelf system using various sensors to accurately detect product presence and manage inventory efficiently. The system communicates real-time data to a web server, providing users with immediate access to product availability through a user-friendly interface. Our solution aims to enhance inventory management, improve customer experience, and streamline restocking processes by combining sensor technology with web connectivity.

## Introduction
Our smart shelf system revolutionizes inventory management in retail environments by utilizing ultrasonic sensors to detect product presence. These sensors emit ultrasonic waves and measure the time it takes for the waves to bounce back from the products. This distance is used to estimate the number of products on the shelf. The data is transmitted to a web server, allowing store managers, staff, and customers to view real-time inventory information. This system eliminates the need for manual stock checks and simplifies the restocking process.

## Hardware Setup
### 1. Hardware Components
- ESP32 Board: Provides Wi-Fi connectivity and processing capabilities.
- Ultrasonic Sensors: Measures the distance between the sensor and products on the shelf.
- Temperature Sensor (DHT11): Monitors the temperature on the shelf.
- Buzzer: Alerts when inventory levels are low.
### 2. Installation
- Setup ESP32 Board: Configure the board to handle Wi-Fi connectivity and processing.
- Install Ultrasonic Sensors: Place sensors on the shelf to detect products and measure distance.
- Install Temperature Sensor: Place the DHT11 sensor on the shelf to measure temperature.

## Data Acquisition
### 1. Ultrasonic Sensors
- Detection: The sensors detect the presence of products by emitting ultrasonic waves and measuring the time for the waves to return.
- Distance Calculation: The distance between the sensor and products is calculated based on the time taken for the waves to bounce back.
### 2. Additional Information
- Shelf ID: Can be processed to distinguish between different shelves.

## Data Transmission to ThingSpeak
### 1. Wi-Fi Connectivity
- Set up Wi-Fi on the ESP32 using the WiFi library to connect to the internet.
### 2. Sending Data
- Use the ThingSpeak library to send sensor readings (temperature, humidity, and quantity of products) to the ThingSpeak cloud platform.

## ThingSpeak Integration
### 1. Publishing Data
- Publish sensor readings to ThingSpeak and visualize the data using charts with timestamps.
### 2. Configuration
- Configure the ESP32 to send data to a specific ThingSpeak channel using the channel ID and API key.

## Results
### Live Tracking
- The system provides real-time tracking of product quantities on the shelf through a line chart on ThingSpeak.
### Web Server 
- Access the web server by opening the IP address, which displays the current temperature, humidity, and product quantity.
## Figures
### Live Tracking Chart
- Shows real-time data of product quantities on the shelf.
###Web Server Display
Shows the web page with temperature, humidity, and product quantity details.
## Code
The provided Arduino code initializes the hardware components, reads sensor data, and transmits the data to ThingSpeak. It also serves a web page with real-time data on product availability, temperature, and humidity.
