#pragma once

#include "../IMUMeasurementStructure.h"
#include "sensor.h"
#include <Ultrasonic.h>

class UltrasonicWrapper : public Sensor {
  private:
    Ultrasonic ultrasonic = Ultrasonic(33);
    unsigned int m_lastTime;

  public:
    String getColumnNames() { return "iterator,seconds,distance\r\n"; }
    String getName() { return "Ultrasonic Sensor"; }
    void begin(MeasurementConfig& config) {
        config.saveAtNDataPoints = 100;
        m_lastTime = millis();
    }
    void gatherAndAccumulateData(String& accumulatedData, MeasurementMetadata& metadata, MeasurementData& data) {
        unsigned int currentTime = millis();
        if (currentTime - m_lastTime >= 50) { // Only log every 500ms
            m_lastTime = currentTime;
            long measureInCentimeters = ultrasonic.MeasureInCentimeters();
            accumulatedData +=
                String(data.iterator) + "," + String(data.seconds) + "," + String(measureInCentimeters) + "\n";
            data.iterator++;
            metadata.nCollectedDataPoints++;
        }
    }
};