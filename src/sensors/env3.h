#pragma once
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
        if (!qmp.begin(&Wire, QMP6988_SLAVE_ADDRESS_L, 32, 33, 400000U)) {
            Serial.println("Couldn't find QMP6988");
        }
        if (!sht3x.begin(&Wire, SHT3X_I2C_ADDR, 32, 33, 400000U)) {
            Serial.println("Couldn't find SHT3X");
        }
    }
    void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) override {
        qmp.update();
        sht3x.update();

        dataLogger.write(data.m_totalSamples);
        dataLogger.write(',');
        dataLogger.write(msSinceStart);
        dataLogger.write(',');
        dataLogger.write(sht3x.humidity, 2);
        dataLogger.write(',');
        dataLogger.write((sht3x.cTemp + qmp.cTemp) / 2, 2);
        dataLogger.write(',');
        dataLogger.write(qmp.pressure, 2);
        dataLogger.write(',');
        dataLogger.write(qmp.altitude, 2);
        dataLogger.write('\n');
    }
};