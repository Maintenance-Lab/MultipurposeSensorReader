#pragma once
#include "../IMUMeasurementStructure.h"
#include "sensor.h"
#include "Wire.h"
#include "../logging.h"
#include <I2C_MPU6886.h>

class IMUWrapper : public Sensor {
  private:
    I2C_MPU6886 m_imu = I2C_MPU6886(I2C_MPU6886_DEFAULT_ADDRESS, Wire);

  public:
    String getColumnNames() override { return "iterator,ms,accX,accY,accZ,gyroX,gyroY,gyroZ\r\n"; }
    String getName() override { return "IMU Sensor"; }
    void begin(MeasurementConfig& config) override {
        config.saveAtNDataPoints = 1000;
        Wire.begin(32, 33);
        m_imu.begin();
    }
    void gatherAndAccumulateData(String& accumulatedData, MeasurementMetadata& metadata,
                                 MeasurementData& data, ulong msSinceStart) override {
        float accX, accY, accZ;
        float gyroX, gyroY, gyroZ;
        float temp;
        m_imu.getAccel(&accX, &accY, &accZ);
        m_imu.getGyro(&gyroX, &gyroY, &gyroZ);
        m_imu.getTemp(&temp);

        // Ensure accumulatedData is a String object and formatted data is
        // concatenated
        LOGLN(data.iterator);
        accumulatedData += String(data.iterator) + "," + String(msSinceStart) + "," + String(accX, 2) + "," +
                           String(accY, 2) + "," + String(accZ, 2) + "," + String(gyroX, 2) + "," + String(gyroY, 2) +
                           "," + String(gyroZ, 2) + "," + String(temp, 2) + "\n";

        data.iterator++;
        metadata.nCollectedDataPoints++;
    }
};