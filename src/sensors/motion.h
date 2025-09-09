#pragma once
#include "sensor.h"

class MotionWrapper : public Sensor {
  private:
    // bool m_detectedLastTime = false;

  public:
    MotionWrapper() {
        numberOfSensors = 1;
        sensorList[0] = {sensingType::motion, true};
    }

    String getName() override { return "Motion Sensor"; }
    void begin(MeasurementConfig& config) override {
        pinMode(36, INPUT);
        beginCalled = true; // to unsure gatherAndAccumulate does not run before begin is called
    }

    void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) override {
        bool detected = (digitalRead(36) == 1);
        // if (detected != m_detectedLastTime) { // Only log if state changed
        // m_detectedLastTime = detected;
        // if (detected) { // Only log if detected, don't log when not detected

        dataLogger.printf("%u,%llu,%d\n", data.m_totalSamples, msSinceStart, detected);
    }
};