#pragma once
#include <WString.h>

struct MeasurementData;
struct MeasurementConfig;

class RenderUI {
    public:
        static void renderStartScreen(int index, String name, bool sdCardExists, int sampleRateHz);
        static void renderStatistics(MeasurementData& data, MeasurementConfig& config, const String& name);
};