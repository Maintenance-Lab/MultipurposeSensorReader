#include "env3.h"

ENV3Wrapper::ENV3Wrapper() {
    sensorList[numberOfSensors++] = {sensingType::humidity, true};
    sensorList[numberOfSensors++] = {sensingType::temperature, true};
    sensorList[numberOfSensors++] = {sensingType::pressure, true};
    sensorList[numberOfSensors++] = {sensingType::altitude, true};
}

void ENV3Wrapper::gatherAndAccumulate(DataLogger& dataLogger, MeasurementData& data, uint64_t msSinceStart) {
    if (!beginCalled) {
        return;
    }

    sht3x.update();

    float pressure = 0.0f;
    float ctemp = sht3x.cTemp;

    dataLogger.printf("%u,%llu", data.m_totalSamples, msSinceStart);
    for (int i = 0; i < numberOfSensors; i++) {
        if (sensorList[i].isActive) {
            switch (sensorList[i].type) {
            case sensingType::humidity:
                dataLogger.printf(",%.2f", sht3x.humidity);
                break;
            case sensingType::temperature:
                ctemp = (sht3x.cTemp + qmp.calcTemperature()) / 2;
                dataLogger.printf(",%.2f", ctemp);
                break;
            case sensingType::pressure:
                pressure = qmp.calcPressure();
                dataLogger.printf(",%.2f", pressure);
                break;
            case sensingType::altitude:
                if (pressure == 0.0f) {
                    pressure = qmp.calcPressure();
                }
                dataLogger.printf(",%.2f", qmp.calcAltitude(pressure, ctemp));
                break;
            }
        }
    }
    dataLogger.write('\n');
}