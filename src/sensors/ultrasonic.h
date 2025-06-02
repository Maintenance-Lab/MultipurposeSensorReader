#pragma once
#include "sensor.h"
#include <Ultrasonic.h>

class UltrasonicWrapper : public Sensor {
  private:
    Ultrasonic ultrasonic = Ultrasonic(33);

  public:
    String getColumnNames() { return "iterator,ms,distance\r\n"; }
    String getName() { return "Ultrasonic Sensor"; }
    void begin(MeasurementConfig& config) {}
    void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) {
        long measureInCentimeters = ultrasonic.MeasureInCentimeters();

        dataLogger.write(data.m_totalSamples);
        dataLogger.write(',');
        dataLogger.write(msSinceStart);
        dataLogger.write(',');
        dataLogger.write(measureInCentimeters, 2);
        dataLogger.write('\n');
    }
};