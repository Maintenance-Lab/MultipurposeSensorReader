#pragma once

#include "../IMUMeasurementStructure.h"
#include "sensor.h"
#include <Ultrasonic.h>

class UltrasonicWrapper : public Sensor {
  private:
    Ultrasonic ultrasonic = Ultrasonic(33);
    unsigned int m_lastTime;

  public:
    String getColumnNames() { return "iterator,ms,distance\r\n"; }
    String getName() { return "Ultrasonic Sensor"; }
    void begin(MeasurementConfig& config) {
        config.setHz = 30;
        config.saveAtNDataPoints = config.setHz * 10; // 33000hz naturally, 300hz with delay, so: 300 * 10s
        m_lastTime = millis();
    }
    void gatherAndAccumulateData(String& accumulatedData, MeasurementConfig& config, MeasurementMetadata& metadata, MeasurementData& data, ulong msSinceStart) {
        unsigned int currentTime = millis();
        if (currentTime - m_lastTime >= (1000 / config.setHz)) { // Log data at the specified frequency
        // if (currentTime - m_lastTime >= 50) { // Only log every 500ms
            m_lastTime = currentTime;
            long measureInCentimeters = ultrasonic.MeasureInCentimeters();
            accumulatedData +=
                String(data.iterator) + "," + String(msSinceStart) + "," + String(measureInCentimeters) + "\n";
            data.iterator++;
            metadata.nCollectedDataPoints++;
        }
    }
};