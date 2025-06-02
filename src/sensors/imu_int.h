#pragma once
#include "Wire.h"
#include "sensor.h"
#include <I2C_MPU6886.h>

class IMUInternalWrapper : public Sensor {
  private:
    I2C_MPU6886 m_imu = I2C_MPU6886(I2C_MPU6886_DEFAULT_ADDRESS, Wire1);

  public:
    String getColumnNames() override { return "iterator,ms,accX,accY,accZ,gyroX,gyroY,gyroZ,temp\r\n"; }
    String getName() override { return "IMU Sensor Internal"; }
    void begin(MeasurementConfig& config) override {
        Wire1.begin(21, 22);
        m_imu.begin();
    }
    void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) override {
        float accX, accY, accZ;
        float gyroX, gyroY, gyroZ;
        float temp;
        m_imu.getAccel(&accX, &accY, &accZ);   // Read accelerometer values
        m_imu.getGyro(&gyroX, &gyroY, &gyroZ); // Read gyroscope values
        m_imu.getTemp(&temp);                  // Read temperature

        dataLogger.write(data.m_totalSamples);
        dataLogger.write(',');
        dataLogger.write(msSinceStart);
        dataLogger.write(',');
        dataLogger.write(accX, 2);
        dataLogger.write(',');
        dataLogger.write(accY, 2);
        dataLogger.write(',');
        dataLogger.write(accZ, 2);
        dataLogger.write(',');
        dataLogger.write(gyroX, 2);
        dataLogger.write(',');
        dataLogger.write(gyroY, 2);
        dataLogger.write(',');
        dataLogger.write(gyroZ, 2);
        dataLogger.write(',');
        dataLogger.write(temp, 2);
        dataLogger.write('\n');
    }
};