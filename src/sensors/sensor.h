#pragma once
#include <M5Core2.h>
#include "../utils/dataLogger.h"
#include "../main.h"

class Sensor {
  public:
    virtual String getColumnNames() = 0;
    virtual String getName() = 0;
    virtual void begin(MeasurementConfig& config) = 0;
    virtual void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) = 0;
};