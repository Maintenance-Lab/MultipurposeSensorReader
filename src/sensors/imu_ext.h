#pragma once
#include "../main.h"
#include "Wire.h"
#include "sensor.h"
#include <I2C_MPU6886.h>

class IMUExternalWrapper : public Sensor {
  private:
    I2C_MPU6886 m_imu = I2C_MPU6886(I2C_MPU6886_DEFAULT_ADDRESS, Wire);

  public:
    IMUExternalWrapper();

    String getName() override { return "IMU Sensor External"; }
    void begin(MeasurementConfig& config) override {
        Wire.begin(32, 33);
        m_imu.begin();
        beginCalled = true; // to unsure gatherAndAccumulate does not run before begin is called
    }
    void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) override;
};