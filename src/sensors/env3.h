#pragma once
#include "../IMUMeasurementStructure.h"
#include "M5UnitENV.h"
#include "sensor.h"

class ENV3Wrapper : public Sensor {
  private:
    QMP6988 qmp;
    SHT3X sht3x;

  public:
    String getColumnNames() override { return "iterator,ms,humidity,temp,pressure,altitude\r\n"; }
    String getName() override { return "Environment Sensor"; }
    void begin(MeasurementConfig& config) override {
        config.setHz = 639;
        config.saveAtNDataPoints = config.setHz * 10; // 639hz * 10s

        if (!qmp.begin(&Wire, QMP6988_SLAVE_ADDRESS_L, 32, 33, 400000U)) {
            Serial.println("Couldn't find QMP6988");
        }
        if (!sht3x.begin(&Wire, SHT3X_I2C_ADDR, 32, 33, 400000U)) {
            Serial.println("Couldn't find SHT3X");
        }
    }
    void gatherAndAccumulateData(String& accumulatedData, MeasurementConfig& config, MeasurementMetadata& metadata,
                                 MeasurementData& data, ulong msSinceStart) override {
        qmp.update();
        sht3x.update();
        accumulatedData += String(data.iterator) + "," + String(msSinceStart) + "," + String(sht3x.humidity, 2) + "," +
                           String((sht3x.cTemp + qmp.cTemp) / 2, 2) + "," + String(qmp.pressure, 2) + "," +
                           String(qmp.altitude, 2) + "\n";
        data.iterator++;
        metadata.nCollectedDataPoints++;
    }
};