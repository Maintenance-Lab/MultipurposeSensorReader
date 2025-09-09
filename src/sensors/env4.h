#pragma once
#include "M5UnitENV.h"
#include "sensor.h"

class ENV4Wrapper : public Sensor {
  private:
    BMP280 bmp280;
    SHT4X sht40;

  public:
    ENV4Wrapper();

    String getName() override { return "Environment Sensor 4"; }

    void begin(MeasurementConfig& config) override {
        if (!bmp280.begin(&Wire, BMP280_I2C_ADDR, 32, 33, 400000U)) {
            Serial.println("Couldn't find BMP280");
        }
        if (!sht40.begin(&Wire, SHT4x_DEFAULT_ADDR, 32, 33, 400000U)) {
            Serial.println("Couldn't find SHT4X");
        }

        beginCalled = true; // to unsure gatherAndAccumulate does not run before begin is called
    }

    void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) override;

  private:
    void updateBMP280() {
        bmp280.readTemperature();
        bmp280.readAltitude();
        /**
         *  manual read because the default update() calls the read pressure twice
         * as readAltitude() also calls readPressure()
         */
    }
};