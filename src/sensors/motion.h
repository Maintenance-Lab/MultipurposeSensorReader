#pragma once
#include "../IMUMeasurementStructure.h"
#include "sensor.h"

class MotionWrapper : public Sensor {
  private:
    bool m_detectedLastTime = false;
    unsigned int m_lastTime = 0;

  public:
    String getColumnNames() override { return "Iterator,ms\r\n"; }
    String getName() override { return "Motion Sensor"; }
    void begin(MeasurementConfig& config) override {
        config.saveAtNDataPoints = 5;
        pinMode(36, INPUT);
        m_lastTime = millis();
    }

    void gatherAndAccumulateData(String& accumulatedData, MeasurementMetadata& metadata,
                                 MeasurementData& data, ulong msSinceStart) override {
        unsigned int currentTime = millis();
        if (currentTime - m_lastTime >= 500) { // Only log every 500ms
            m_lastTime = currentTime;

            bool detected = digitalRead(36) == 1;
            if (detected != m_detectedLastTime) { // Only log if state changed
                m_detectedLastTime = detected;
                if (detected) { // Only log if detected, don't log when not detected
                    accumulatedData += String(data.iterator) + "," + String(msSinceStart) + "\n";
                    data.iterator++;
                    metadata.nCollectedDataPoints++;
                }
            }
        }
    }
};