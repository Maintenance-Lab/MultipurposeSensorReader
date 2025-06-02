#pragma once
#include "sensor.h"

class MotionWrapper : public Sensor {
  private:
    // bool m_detectedLastTime = false;

  public:
    String getColumnNames() override { return "Iterator,ms,detected\r\n"; }
    String getName() override { return "Motion Sensor"; }
    void begin(MeasurementConfig& config) override {
        pinMode(36, INPUT);
    }

    void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) override {
        bool detected = digitalRead(36) == 1;
        // if (detected != m_detectedLastTime) { // Only log if state changed
        // m_detectedLastTime = detected;
        // if (detected) { // Only log if detected, don't log when not detected

        dataLogger.write(data.m_totalSamples);
        dataLogger.write(',');
        dataLogger.write(msSinceStart);
        dataLogger.write(',');
        dataLogger.write(detected);
        dataLogger.write('\n');

        // delay(500);
        // }
        // }
    }
};