#pragma once
#include "../IMUMeasurementStructure.h"
#include "sensor.h"

class MotionWrapper : public Sensor {
  private:
    // bool m_detectedLastTime = false;
    unsigned int m_lastTime = 0;

  public:
    String getColumnNames() override { return "Iterator,ms,detected\r\n"; }
    String getName() override { return "Motion Sensor"; }
    void begin(MeasurementConfig& config) override {
        config.setHz = 300;
        config.saveAtNDataPoints = config.setHz * 10; // 36506hz naturally, 300hz with delay, so: 300 * 10s
        pinMode(36, INPUT);
        m_lastTime = millis();
    }

    void gatherAndAccumulateData(FileWriter& fileWriter, MeasurementConfig& config, MeasurementMetadata& metadata,
                                 MeasurementData& data, ulong msSinceStart) override {
                                    
        unsigned int currentTime = millis();
        if (currentTime - m_lastTime >= (1000 / config.setHz)) { // Log data at the specified frequency
        // if (currentTime - m_lastTime >= 10) { // Only log every 100ms
            m_lastTime = currentTime;

            bool detected = digitalRead(36) == 1;
            // if (detected != m_detectedLastTime) { // Only log if state changed
                // m_detectedLastTime = detected;
                // if (detected) { // Only log if detected, don't log when not detected

            fileWriter.write(data.iterator);
            fileWriter.write(',');
            fileWriter.write(msSinceStart);
            fileWriter.write(',');
            fileWriter.write(detected);
            fileWriter.write('\n');

            data.iterator++;
            // delay(500);
                // }
            // }
        }
    }
};