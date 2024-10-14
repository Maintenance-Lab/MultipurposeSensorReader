# MultipurposeSensorReader
 
This repository contains a PlatformIO-based solution for the M5Stack Core2 to measure and collect data from various sensors. The project is designed to work with the following sensors:
* [ENV3 (temperature, humidity, preasure)](https://docs.m5stack.com/en/unit/envIII)
* [IMU (inertia measurement unit)](https://docs.m5stack.com/en/unit/IMU)
* [Motion](https://docs.m5stack.com/en/unit/pir)
* [Ultrasonic](https://docs.m5stack.com/en/unit/ULTRASONIC%20I2C)
  
The MultipurposeSensorReader is an all-in-one solution for collecting environmental and motion data, making it ideal for IoT applications, data logging, and automation projects.

# Installation
1. Clone this repository:
```bash
git clone https://github.com/Maintenance-Lab/MultipurposeSensorReader.git
cd MultipurposeSensorReader
```
2. Open the project in PlatformIO (VSCode):
* Launch VSCode, open the cloned folder, and PlatformIO will detect it as a project.
3. Upload the project to the M5Stack Core2:
* Connect your M5Stack Core2 via USB.
* Click the "PlatformIO: Upload" button in the VSCode toolbar or run:
```bash
pio run --target upload
```
