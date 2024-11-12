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
    void gatherAndAccumulateData(String& accumulatedData, MeasurementConfig& config, MeasurementMetadata& metadata,
                                 MeasurementData& data, ulong msSinceStart) override {
        float accX, accY, accZ;
        float gyroX, gyroY, gyroZ;
        float temp;
        m_imu.getAccel(&accX, &accY, &accZ);
        m_imu.getGyro(&gyroX, &gyroY, &gyroZ);
        m_imu.getTemp(&temp);

        // Ensure accumulatedData is a String object and formatted data is  concatenated
        accumulatedData += String(data.iterator) + "," + String(msSinceStart) + "," + String(accX, 2) + "," +
                           String(accY, 2) + "," + String(accZ, 2) + "," + String(gyroX, 2) + "," + String(gyroY, 2) +
                           "," + String(gyroZ, 2) + "," + String(temp, 2) + "\n";

        data.iterator++;
        metadata.nCollectedDataPoints++;
    }
};