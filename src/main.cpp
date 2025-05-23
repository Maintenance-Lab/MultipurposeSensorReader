#include "AXP192.h"
#include "IMUMeasurementStructure.h"
#include "esp32_digital_led_lib.h"
#include "logging.h"
#include "renders/IMU_render.h"
#include "sensors/env3.h"
#include "sensors/imu_ext.h"
#include "sensors/imu_int.h"
#include "sensors/motion.h"
#include "sensors/sensor.h"
#include "sensors/ultrasonic.h"
#include "ultrasonic.h"
#include "utils/SD_controller.h"
#include <M5Core2.h>

String accumulatedData;
const unsigned int timeInterval = 1000;
int indexMeasurementSession = 0;
unsigned int lastTime = 0;
ulong measurementStartTime = 0;
bool isMeasuring = false;
bool buttonsRendered = false;
Sensor* sensor;
MeasurementData data;
MeasurementMetadata metadata;
MeasurementConfig config;

void m5SetDefaultSettings() {
    M5.Lcd.setTextSize(2);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Axp.SetLcdVoltage(2700); //3000 very bright, 2500 way too dark, 2600 very dim
}

void setup() {
    M5.begin();
    INITLOG();
    m5SetDefaultSettings();
    indexMeasurementSession = SD_controller::countNumberOfFiles();
    sensor = new ENV3Wrapper();
}

int sensorTypeIndex = 0;
void switchSensorType() {
    if (sensorTypeIndex > 4) {
        sensorTypeIndex = 0;
    }
    delete sensor;
    switch (sensorTypeIndex) {
    case 0:
        sensor = new UltrasonicWrapper();
        break;
    case 1:
        sensor = new IMUExternalWrapper();
        break;
    case 2:
        sensor = new IMUInternalWrapper();
        break;
    case 3:
        sensor = new MotionWrapper();
        break;
    case 4:
        sensor = new ENV3Wrapper();
        break;
    }
    sensorTypeIndex++;
}

void saveData(MeasurementMetadata& metadata, MeasurementConfig& config) {
    metadata.measurementsIndex += metadata.nCollectedDataPoints;
    SD_controller::appendFile(indexMeasurementSession, accumulatedData, sensor->getColumnNames().c_str());

    // Reset metadata
    metadata.nCollectedDataPoints = 0;
    accumulatedData = "";
}

void updateDisplay(MeasurementData& data, MeasurementMetadata& metadata, MeasurementConfig& config) {
    M5.Lcd.fillRect(0, 0, 320, 200, BLACK);

    IMU_RENDER::renderStatistics(metadata, config, data);
}

void startMeasurement() {
    measurementStartTime = millis();
    accumulatedData = "";
    resetMeasurement(data, metadata);
    sensor->begin(config);
}

void checkButtonEvent() {
    M5.update();

    if (M5.BtnA.wasPressed()) {
        isMeasuring = !isMeasuring;
        if (isMeasuring) {
            startMeasurement();
        } else {
            accumulatedData = "";
            resetMeasurement(data, metadata);
        }
        buttonsRendered = false;
        indexMeasurementSession = SD_controller::countNumberOfFiles();
    }

    if (M5.BtnC.wasPressed()) {
        switchSensorType();
        buttonsRendered = false;
    }
}

void loop() {
    if (isMeasuring) {
        unsigned int currentTime = millis();

        if (!buttonsRendered) { // check if button was already rendered to avoid flickering
            M5.Lcd.fillRect(0, 0, 320, 240, BLACK);
            IMU_RENDER::renderMeasuringScreen();

            resetMeasurement(data, metadata);
            buttonsRendered = true;
        }

        if (currentTime - lastTime >= timeInterval) {
            updateDisplay(data, metadata, config);
            lastTime = currentTime;
            data.seconds++;
            metadata.hz = 0;
        }

        sensor->gatherAndAccumulateData(accumulatedData, config, metadata, data, millis() - measurementStartTime);

        // Save data if needed when the number of data points is reached
        if (metadata.nCollectedDataPoints >= config.saveAtNDataPoints) {
            saveData(metadata, config);
        }

        metadata.hz++;
    } else {
        if (!buttonsRendered) { // check if button was already rendered to avoid flickering
            M5.Lcd.fillRect(0, 0, 320, 240, BLACK);
            IMU_RENDER::renderStartScreen(indexMeasurementSession, sensor->getName());
            buttonsRendered = true;
        }
    }
    checkButtonEvent();
}