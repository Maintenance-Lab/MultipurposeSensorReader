#include "IMU_render.h"
#include "../IMUMeasurementStructure.h"
#include "../utils/SD_controller.h"

int getBatteryPercentage() {
    float batVoltage = M5.Axp.GetBatVoltage();
    return ( batVoltage < 3.2 ) ? 0 : ( batVoltage - 3.2 ) * 100;
}

void IMU_RENDER::renderStartScreen(int index, String name) {
    M5.Lcd.setCursor(70, 5);
    M5.Lcd.printf("Maintenance Lab");
    
    M5.Lcd.setCursor(280, 5);
    M5.Lcd.printf("%d%%", getBatteryPercentage());

    M5.Lcd.drawCentreString(name, 160, 50, 2);

    if (SD_controller::exists()) {
        M5.Lcd.setCursor(10, 150);
        M5.Lcd.printf("%d loggings found", index);
        M5.Lcd.setCursor(30, 170);
        M5.Lcd.printf("on MicroSDHC card");
    } else {
        M5.Lcd.setCursor(30, 150);
        M5.Lcd.printf("No SD card found");
    }

    M5.Lcd.setCursor(20, 240 - 20);
    M5.Lcd.printf("Start");

    M5.Lcd.setCursor(235, 240 - 20);
    M5.Lcd.printf("Change");
}

void IMU_RENDER::renderMeasuringScreen() {
    M5.Lcd.setCursor(30, 240 - 20);
    M5.Lcd.printf("Stop");
}

void IMU_RENDER::renderStatistics(MeasurementMetadata& metadata, MeasurementConfig& config, MeasurementData& data) {
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.printf("%d s", data.seconds);
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.printf("%dhz (possible: %dhz)", config.setHz, metadata.hz);
    M5.Lcd.setCursor(0, 60);
    M5.Lcd.printf("%d iterator", data.iterator);

    M5.Lcd.setCursor(0, 100);
    M5.Lcd.printf("%d/%d gathered (buffer) ", metadata.nCollectedDataPoints, config.saveAtNDataPoints);
    M5.Lcd.setCursor(0, 120);
    M5.Lcd.printf("%d saved (%d batches)", metadata.measurementsIndex, metadata.measurementsIndex / config.saveAtNDataPoints);
    M5.Lcd.setCursor(0, 140);
    M5.Lcd.printf("%d total", data.iterator);

    M5.Lcd.setCursor(280, 5);
    M5.Lcd.printf("%d%%", getBatteryPercentage());
}