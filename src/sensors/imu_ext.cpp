#include "imu_ext.h"

IMUExternalWrapper::IMUExternalWrapper() {
    this->sensorList[numberOfSensors++] = {sensingType::accelerometer, true};
    this->sensorList[numberOfSensors++] = {sensingType::gyro, true};
    this->sensorList[numberOfSensors++] = {sensingType::temperature, true};
}

void IMUExternalWrapper::gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) {
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
    float temp;

    dataLogger.printf("%u,%llu", data.m_totalSamples, msSinceStart);
    for (int i = 0; i < numberOfSensors; i++) {
        if (sensorList[i].isActive) {
            switch (sensorList[i].type) {
            case sensingType::accelerometer:
                m_imu.getAccel(&accX, &accY, &accZ); // Read accelerometer values
                dataLogger.printf(",%.2f,%.2f,%.2f", accX, accY, accZ);
                break;
            case sensingType::gyro:
                m_imu.getGyro(&gyroX, &gyroY, &gyroZ); // Read gyroscope values
                dataLogger.printf(",%.2f,%.2f,%.2f", gyroX, gyroY, gyroZ);
                break;
            case sensingType::temperature:
                m_imu.getTemp(&temp); // Read temperature
                dataLogger.printf(",%.2f", temp);
                break;
            }
        }
    }
    dataLogger.write('\n');
}
