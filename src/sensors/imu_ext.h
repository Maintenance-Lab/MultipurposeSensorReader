#pragma once
#include "../IMUMeasurementStructure.h"
#include "../logging.h"
#include "Wire.h"
#include "sensor.h"
#include <I2C_MPU6886.h>

class IMUExternalWrapper : public Sensor {
  private:
    I2C_MPU6886 m_imu = I2C_MPU6886(I2C_MPU6886_DEFAULT_ADDRESS, Wire);

  public:
    String getColumnNames() override { return "iterator,ms,accX,accY,accZ,gyroX,gyroY,gyroZ,temp\r\n"; }
    String getName() override { return "IMU Sensor External"; }
    void begin(MeasurementConfig& config) override {
        config.setHz = 130;
        config.saveAtNDataPoints = config.setHz * 10; // 130hz * 10s
        Wire.begin(32, 33);
        m_imu.begin();
    }
    void gatherAndAccumulateData(FileWriter& fileWriter, MeasurementConfig& config, MeasurementMetadata& metadata,
                                 MeasurementData& data, ulong msSinceStart) override {
        float accX, accY, accZ;
        float gyroX, gyroY, gyroZ;
        float temp;
        m_imu.getAccel(&accX, &accY, &accZ);
        m_imu.getGyro(&gyroX, &gyroY, &gyroZ);
        m_imu.getTemp(&temp);

        fileWriter.write(data.iterator);
        fileWriter.write(',');
        fileWriter.write(msSinceStart);
        fileWriter.write(',');
        fileWriter.write(accX, 2);
        fileWriter.write(',');
        fileWriter.write(accY, 2);
        fileWriter.write(',');
        fileWriter.write(accZ, 2);
        fileWriter.write(',');
        fileWriter.write(gyroX, 2);
        fileWriter.write(',');
        fileWriter.write(gyroY, 2);
        fileWriter.write(',');
        fileWriter.write(gyroZ, 2);
        fileWriter.write(',');
        fileWriter.write(temp, 2);
        fileWriter.write('\n');

        data.iterator++;
        metadata.nCollectedDataPoints++;
    }
};