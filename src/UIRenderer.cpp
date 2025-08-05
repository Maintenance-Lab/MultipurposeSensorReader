#include "UIRenderer.h"
#include "main.h"

#define MAX_MENU_ITEMS 5

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

        M5.Lcd.setCursor(110, 240 - 20);
        M5.Lcd.printf("SETTINGS");

        M5.Lcd.setCursor(230, 220 - 20);
        M5.Lcd.printf("DETECT");
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

void RenderUI::renderSettings(char menuItems[][MENU_STRING_LENGTH], int idx, size_t lengthMenuItems) {
    drawTitle("");
    drawBattery();

    M5.Lcd.setCursor(20, 240 - 20);
    M5.Lcd.printf("SELECT");

    M5.Lcd.setCursor(150, 240 - 20);
    M5.Lcd.printf("UP");

    M5.Lcd.setCursor(240, 240 - 20);
    M5.Lcd.printf("DOWN");

    int items = MAX_MENU_ITEMS;
    if (lengthMenuItems < MAX_MENU_ITEMS) {
        items = lengthMenuItems;
    }

    int startIdx = 0;
    int endOfList = lengthMenuItems -MAX_MENU_ITEMS;
    if (idx < MAX_MENU_ITEMS) {
        startIdx = 0;
    } else if (idx >= endOfList) {
        startIdx = endOfList;
    } else {
        startIdx = idx - 2;
    }

    for (int i = startIdx; i < (items + startIdx); i++) {
        M5.Lcd.setCursor(10, 50 + (i -startIdx) * 20);
        if (i == idx) {
            M5.Lcd.print("-> ");
        }
        M5.Lcd.print(menuItems[i]);
    }
}
