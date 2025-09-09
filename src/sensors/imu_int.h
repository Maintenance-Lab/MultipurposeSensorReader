#pragma once
#include "Wire.h"
#include "sensor.h"
#include <I2C_MPU6886.h>

class IMUInternalWrapper : public Sensor {
  private:
    I2C_MPU6886 m_imu = I2C_MPU6886(I2C_MPU6886_DEFAULT_ADDRESS, Wire1);

  public:
    IMUInternalWrapper();

    String getName() override { return "IMU Sensor Internal"; }
    void begin(MeasurementConfig& config) override {
        Wire1.begin(21, 22);
        m_imu.begin();
        beginCalled = true; // to unsure gatherAndAccumulate does not run before begin is called
    }

    void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) override;
};