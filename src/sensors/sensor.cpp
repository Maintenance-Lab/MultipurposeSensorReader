#include "sensor.h"

String Sensor::getColumnNames()
{
    String columnNames = "iterator,ms";
    for (int i = 0; i < numberOfSensors; i++) {
        if (sensorList[i].isActive) {
            columnNames += ",";
            int sensorID = static_cast<int>(sensorList[i].type) -1;
            columnNames += sensorUnits[sensorID];
        }
    }
    return columnNames + "\r\n";
}

const char* Sensor::getSensorList(int index) const {
    int sensorID = static_cast<int>(sensorList[index].type) -1;
    return sensorID < 0 ? "No Sensor" : sensorNames[sensorID];
}

void Sensor::toggleSensorState(int index) {
    if (index >= 0 && index < numberOfSensors) {
        sensorList[index].isActive = !sensorList[index].isActive;
    }
}

int Sensor::getNumberOfSensors() const { return numberOfSensors; }

bool Sensor::isSensorActive(int index) const {
    if (index >= 0 && index < numberOfSensors) {
        return sensorList[index].isActive;
    }
    return false; // Invalid index
}
