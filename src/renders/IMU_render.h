#ifndef IMU_RENDER_H
#define IMU_RENDER_H

#include <M5Core2.h>
#include "../IMUMeasurementStructure.h"

class IMU_RENDER {
    public:
        static void renderStartScreen(int index, String name, bool sdCardExists);
        static void renderMeasuringScreen();
        static void renderStatistics( MeasurementMetadata& metadata, MeasurementConfig& config, MeasurementData& data);
};

#endif // IMU_RENDER_H