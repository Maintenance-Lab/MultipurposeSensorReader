#include "IMU_render.h"
#include "../IMUMeasurementStructure.h"
#include "../utils/SD_controller.h"

void IMU_RENDER::renderStartScreen(int index, String name) {
    M5.Lcd.setCursor(70, 5);
    M5.Lcd.printf("Maintenance Lab");

    M5.Lcd.drawCentreString(name, 160, 30, 2);

    if (SD_controller::exists()) {
        M5.Lcd.setCursor(10, 100);
        M5.Lcd.printf("%d loggings found", index);
        M5.Lcd.setCursor(30, 120);
        M5.Lcd.printf("on SD card");
    } else {
        M5.Lcd.setCursor(30, 100);
        M5.Lcd.printf("No SD card found");
    }

    M5.Lcd.setCursor(20, 240 - 20);
    M5.Lcd.printf("Start");
}

void IMU_RENDER::renderMeasuringScreen() {
    M5.Lcd.setCursor(30, 240 - 20);
    M5.Lcd.printf("Stop");
}

void IMU_RENDER::renderStatistics(MeasurementMetadata& metadata, MeasurementConfig& config, MeasurementData& data) {
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.printf("%d s", data.seconds);
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.printf("%d hz", metadata.hz);
    M5.Lcd.setCursor(0, 60);
    M5.Lcd.printf("%d iterator", data.iterator);

    M5.Lcd.setCursor(0, 100);
    M5.Lcd.printf("%d gathered", metadata.nCollectedDataPoints);
    M5.Lcd.setCursor(0, 120);
    M5.Lcd.printf("%d saved (%d)", metadata.measurementsIndex, metadata.measurementsIndex / config.saveAtNDataPoints);
    M5.Lcd.setCursor(0, 140);
    M5.Lcd.printf("%d total", data.iterator);
}