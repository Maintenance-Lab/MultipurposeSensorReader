#include "UIRenderer.h"
#include "main.h"

int getBatteryPercentage() {
    float batVoltage = M5.Axp.GetBatVoltage();
    if (batVoltage < 3.2f)
        return 0;
    int pct = int((batVoltage - 3.2f) * 100.0f);
    if (pct > 100)
        pct = 100;
    return pct;
}

void drawTitle(const String& name) {
    M5.Lcd.setCursor(70, 5);
    M5.Lcd.printf("Maintenance Lab");

    M5.Lcd.drawCentreString(name, 160, 50, 2);
}

void drawBattery() {
    int batteryPct = getBatteryPercentage();

    if (batteryPct < 20) {
        M5.Lcd.setTextColor(RED);
    } else if (M5.Axp.isCharging()) {
        M5.Lcd.setTextColor(GREEN);
    }

    M5.Lcd.setCursor(280, 5);
    M5.Lcd.printf("%d%%", getBatteryPercentage());
    M5.Lcd.setTextColor(WHITE);
}

void RenderUI::renderStartScreen(int index, String name, bool sdCardExists, int sampleRateHz) {
    drawTitle(name);
    drawBattery();

    M5.Lcd.setCursor(10, 125);
    M5.Lcd.printf("Sample Rate: %d Hz", sampleRateHz);

    if (sdCardExists) {
        M5.Lcd.setCursor(10, 150);
        M5.Lcd.printf("Found %d log files", index);
        M5.Lcd.setCursor(30, 170);
        M5.Lcd.printf("on MicroSDHC card");

        M5.Lcd.setCursor(20, 240 - 20);
        M5.Lcd.printf("START");

        M5.Lcd.setCursor(140, 240 - 20);
        M5.Lcd.printf("RATE");

        M5.Lcd.setCursor(230, 240 - 20);
        M5.Lcd.printf("SENSOR");
    } else {
        M5.Lcd.setCursor(30, 150);
        M5.Lcd.printf("No SD card found");
    }
}

void RenderUI::renderStatistics(MeasurementData& data, MeasurementConfig& config, const String& name) {
    drawTitle(name);
    drawBattery();

    M5.Lcd.setCursor(10, 100);
    M5.Lcd.printf("%d seconds", data.m_elapsedSeconds);
    M5.Lcd.setCursor(10, 120);
    M5.Lcd.printf("%d samples per second", data.m_samplesThisSecond);
    M5.Lcd.setCursor(10, 140);
    M5.Lcd.printf("%d total samples", data.m_totalSamples);

    M5.Lcd.setCursor(280, 5);
    M5.Lcd.printf("%d%%", getBatteryPercentage());

    M5.Lcd.setCursor(30, 240 - 20);
    M5.Lcd.printf("STOP");
}