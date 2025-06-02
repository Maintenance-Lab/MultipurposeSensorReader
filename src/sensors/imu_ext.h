#pragma once
#include "Wire.h"
#include "sensor.h"
#include <I2C_MPU6886.h>
#include "../main.h"

class IMUExternalWrapper : public Sensor {
  private:
    I2C_MPU6886 m_imu = I2C_MPU6886(I2C_MPU6886_DEFAULT_ADDRESS, Wire);

  public:
    String getColumnNames() override { return "iterator,ms,accX,accY,accZ,gyroX,gyroY,gyroZ,temp\r\n"; }
    String getName() override { return "IMU Sensor External"; }
    void begin(MeasurementConfig& config) override {
        Wire.begin(32, 33);
        m_imu.begin();
    }
    void gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) override {
        float accX, accY, accZ;
        float gyroX, gyroY, gyroZ;
        float temp;
        m_imu.getAccel(&accX, &accY, &accZ);
        m_imu.getGyro(&gyroX, &gyroY, &gyroZ);
        m_imu.getTemp(&temp);

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