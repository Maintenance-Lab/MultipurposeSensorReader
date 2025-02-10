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
    void gatherAndAccumulateData(FileWriter& fileWriter, MeasurementConfig& config, MeasurementMetadata& metadata,
                                 MeasurementData& data, ulong msSinceStart) override {
        qmp.update();
        sht3x.update();

        fileWriter.write(data.iterator);
        fileWriter.write(',');
        fileWriter.write(msSinceStart);
        fileWriter.write(',');
        fileWriter.write(sht3x.humidity, 2);
        fileWriter.write(',');
        fileWriter.write((sht3x.cTemp + qmp.cTemp) / 2, 2);
        fileWriter.write(',');
        fileWriter.write(qmp.pressure, 2);
        fileWriter.write(',');
        fileWriter.write(qmp.altitude, 2);
        fileWriter.write('\n');

        data.iterator++;
        metadata.nCollectedDataPoints++;
    }
};