#include "env4.h"

ENV4Wrapper::ENV4Wrapper() {
    sensorList[numberOfSensors++] = {sensingType::humidity, true};
    sensorList[numberOfSensors++] = {sensingType::temperature, true};
    sensorList[numberOfSensors++] = {sensingType::pressure, true};
    sensorList[numberOfSensors++] = {sensingType::altitude, true};
}

void ENV4Wrapper::gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) {
    sht40.update();

    // as pressure is required for altitude this we do the read before looping through the sensors
    if (sensorList[3].isActive) { // index of altitude sensor in sensorList
        bmp280.readAltitude();
    } else if (sensorList[2].isActive) { // index of pressure sensor in sensorList
        bmp280.readPressure();
    }

    dataLogger.printf("%d,%llu", data.m_totalSamples, msSinceStart);
    for (int i = 0; i < numberOfSensors; i++) {
        if (sensorList[i].isActive) {
            switch (sensorList[i].type) {
            case sensingType::humidity:
                dataLogger.printf(",%.2f", sht40.humidity);
                break;
            case sensingType::temperature:
                bmp280.readTemperature();
                dataLogger.printf(",%.2f", (sht40.cTemp + bmp280.cTemp) / 2);
                break;
            case sensingType::pressure:
                dataLogger.printf(",%.2f", bmp280.pressure);
                break;
            case sensingType::altitude:
                dataLogger.printf(",%.2f", bmp280.altitude);
                break;
            }
        }
    }

    dataLogger.write('\n');
}