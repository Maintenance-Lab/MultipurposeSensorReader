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
#include "utils/fileSystem.h"
#include "utils/fileWriter.h"
#include <M5Core2.h>

constexpr static uint32_t FLUSH_INTERVAL = 1000;

static uint32_t s_indexMeasurementSession = 0;
static ulong s_measurementStartTime = 0;
static bool s_isMeasuring = false;
static bool s_shouldRender = true;
static Sensor* s_activeSensor;
static MeasurementData data;
static MeasurementMetadata metadata;
static MeasurementConfig config;
static FileWriter fileWriter(FLUSH_INTERVAL);
static bool s_sdCardExists = false;

void m5SetDefaultSettings() {
    M5.Lcd.setTextSize(2);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Axp.SetLcdVoltage(2700); // 3000 very bright, 2500 way too dark, 2600 very dim
}

void setup() {
    M5.begin();
    INITLOG();
    m5SetDefaultSettings();
    s_indexMeasurementSession = FileSystem::countNumberOfFiles();
    s_activeSensor = new ENV3Wrapper();
}

int sensorTypeIndex = 0;
void switchSensorType() {
    if (sensorTypeIndex > 4) {
        sensorTypeIndex = 0;
    }
    delete s_activeSensor;
    switch (sensorTypeIndex) {
    case 0:
        s_activeSensor = new UltrasonicWrapper();
        break;
    case 1:
        s_activeSensor = new IMUExternalWrapper();
        break;
    case 2:
        s_activeSensor = new IMUInternalWrapper();
        break;
    case 3:
        s_activeSensor = new MotionWrapper();
        break;
    case 4:
        s_activeSensor = new ENV3Wrapper();
        break;
    }
    sensorTypeIndex++;
}

void updateDisplay(MeasurementData& data, MeasurementMetadata& metadata, MeasurementConfig& config) {
    M5.Lcd.fillRect(0, 0, 320, 200, BLACK);

    IMU_RENDER::renderStatistics(metadata, config, data);
}

void startMeasurement() {
    s_measurementStartTime = millis();
    resetMeasurement(data, metadata);
    fileWriter.open(s_activeSensor->getColumnNames().c_str());
    s_activeSensor->begin(config);
}

void checkButtonEvent() {
    M5.update();

    if (s_sdCardExists && M5.BtnA.wasPressed()) {
        s_isMeasuring = !s_isMeasuring;
        s_indexMeasurementSession = FileSystem::countNumberOfFiles();

        if (s_isMeasuring) {
            startMeasurement();
        } else {
            resetMeasurement(data, metadata);
            fileWriter.close();
        }
        s_shouldRender = true;
    }

    if (M5.BtnC.wasPressed()) {
        switchSensorType();
        s_shouldRender = true;
    }
}

// This does not work since the M5stack does not properly detect changes. Needs work.
void checkForSDCardChange(){
    bool sdCardExists = FileSystem::exists();
    if (sdCardExists != s_sdCardExists) {
        s_sdCardExists = sdCardExists;
        s_shouldRender = true;
    }
}

void render(){
    if (s_shouldRender) {
        M5.Lcd.fillRect(0, 0, 320, 240, BLACK);
        if (s_isMeasuring) {
            IMU_RENDER::renderMeasuringScreen();
            resetMeasurement(data, metadata);
        } else {
            IMU_RENDER::renderStartScreen(s_indexMeasurementSession, s_activeSensor->getName(), s_sdCardExists);
        }
        s_shouldRender = false;
    }
}

void loop() {
    checkForSDCardChange();

    if (s_isMeasuring) {
        uint32_t currentTime = millis();

        static uint32_t lastTime = 0;
        if (currentTime - lastTime >= 1000) { // Every second
            updateDisplay(data, metadata, config);
            lastTime = currentTime;
            data.seconds++;
            metadata.hz = 0;
        }

        s_activeSensor->gatherAndAccumulateData(fileWriter, config, metadata, data, currentTime - s_measurementStartTime);

        metadata.hz++;
    }
    checkButtonEvent();
    render();

    fileWriter.update();
}