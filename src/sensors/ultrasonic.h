#pragma once
#include "sensor.h"
#include <Ultrasonic.h>

class UltrasonicWrapper : public Sensor {
  private:
    Ultrasonic ultrasonic = Ultrasonic(33);

  public:
    UltrasonicWrapper() {
        numberOfSensors = 1;
        sensorList[0] = {sensingType::distance, true};
    }

    String getName() { return "Ultrasonic Sensor"; }
    void begin(MeasurementConfig& config) {}
    void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) {
        long measureInCentimeters = ultrasonic.MeasureInCentimeters();

        dataLogger.printf("%u,%llu,%.2ld\n", data.m_totalSamples, msSinceStart, measureInCentimeters);
    }
};