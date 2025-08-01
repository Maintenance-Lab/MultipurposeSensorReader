#pragma once
#include <WString.h>
#include "sensors/sensor.h"

struct MeasurementData;
struct MeasurementConfig;

#define MENU_STRING_LENGTH 30

class RenderUI {
    public:
        static void renderStartScreen(int index, String name, bool sdCardExists, int sampleRateHz);
        static void renderStatistics(MeasurementData& data, MeasurementConfig& config, const String& name);
        static void renderSettings(char menuItems[][MENU_STRING_LENGTH], int idx, size_t lengthMenuItems);
};