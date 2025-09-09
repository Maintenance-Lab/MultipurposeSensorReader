#pragma once
#include "M5UnitENV.h"
#include "sensor.h"

class ENV3Wrapper : public Sensor {
  private:
    QMP6988 qmp;
    SHT3X sht3x;

  public:
    ENV3Wrapper();

    String getName() override { return "Environment Sensor 3"; }
    void begin(MeasurementConfig& config) override {
        if (!qmp.begin(&Wire, QMP6988_SLAVE_ADDRESS_L, 32, 33, 400000U)) {
            Serial.println("Couldn't find QMP6988");
        }
        if (!sht3x.begin(&Wire, SHT3X_I2C_ADDR, 32, 33, 400000U)) {
            Serial.println("Couldn't find SHT3X");
        }

        beginCalled = true; // to unsure gatherAndAccumulate does not run before begin is called
    }
    void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) override;
};