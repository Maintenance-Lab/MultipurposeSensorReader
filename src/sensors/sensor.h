#pragma once
#include "../IMUMeasurementStructure.h"
#include <M5Core2.h>

class Sensor {
  public:
    virtual String getColumnNames() = 0;
    virtual String getName() = 0;
    virtual void begin(MeasurementConfig& config) = 0;
    virtual void gatherAndAccumulateData(String& accumulatedData, MeasurementMetadata& metadata, MeasurementData& data, ulong msSinceStart);
};